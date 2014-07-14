#include <QDebug>
#include <QtNetwork>
#include <QtCore>
#include "cbaidutranslater.h"


CBaiduTranslater::CBaiduTranslater(QString API_Key, QString url, QObject *parent)
    :QObject(parent), m_url(url),
     m_API_Key(API_Key), m_from("auto"), m_to("auto"), m_q(""),
     m_networkAccessManager(new QNetworkAccessManager(this))
{
}

QString CBaiduTranslater::API_Key() const
{
    return m_API_Key;
}

void CBaiduTranslater::setAPI_Key(const QString &API_Key)
{
    m_API_Key = API_Key;
    m_client_id = m_API_Key;        // client_id equal to API key
}
QString CBaiduTranslater::from() const
{
    return m_from;
}

void CBaiduTranslater::setFrom(const QString &from)
{
    m_from = from;
}
QString CBaiduTranslater::to() const
{
    return m_to;
}

void CBaiduTranslater::setTo(const QString &to)
{
    m_to = to;
}
QString CBaiduTranslater::q() const
{
    return m_q;
}

void CBaiduTranslater::setQ(const QString &q)
{
    m_q = q;
}

void CBaiduTranslater::translate(const QString &src)
{
    translate(src, m_from, m_to);
}

void CBaiduTranslater::translate()
{
    translate(m_q, m_from, m_to);
}

void CBaiduTranslater::translate(const QString &src, const QString from, const QString to)
{
    if (m_networkAccessManager == nullptr)
        return;

    /* create query url */
    QUrl url(m_url);
    QUrlQuery query;
    query.addQueryItem("from", from);
    query.addQueryItem("to", to);
    query.addQueryItem("client_id", m_client_id);
    query.addQueryItem("q", src);
    url.setQuery(query);

    // create http request header
    QNetworkRequest request(url);

    // send GET request to get result
    QNetworkReply *reply = m_networkAccessManager->get(request);

    /* return data */
    connect(reply, &QNetworkReply::readyRead, [=]{
        QByteArray data = reply->readAll();
        bool ok = false;
        QVector<QPair<QString, QString> > vector = decodeJsonData(data, ok);
        if (ok)
        {
            emit finished(vector);
        }
        else
        {
            QMap<QString, QString> map;
            for (auto iter = vector.begin(); iter != vector.end(); ++iter)
            {
                QPair<QString, QString> pair = static_cast<QPair<QString, QString> >(*iter);
                map.insert(pair.first, pair.second);
            }
            emit error(map);
        }
        reply->close();
    });

    /* finish request */
    connect(reply, &QNetworkReply::finished, [=]{
        reply->deleteLater();
    });

    /* error */
    auto pError = static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error);
    connect(reply, pError, [=](QNetworkReply::NetworkError){
        reply->deleteLater();
    });
}

/*
// normal result
{
    "from": "en",
    "to": "zh",
    "trans_result": [
        {
            "src": "today",
            "dst": "今天"
        },
        {
            "src": "tomorrow",
            "dst": "明天"
        }
    ]
}

// curr error
{
    "error_code": "52001",
    "error_msg": "TIMEOUT",
    "from": "auto",
    "to": "auto",
    "query": "he's"
}

if decode successfully return hash table containing <src, dst>
else return hash table containing <error_msg, error_msg>
*/
QVector<QPair<QString, QString> > CBaiduTranslater::decodeJsonData(const QByteArray &json, bool &ok)
{
    QVector<QPair<QString, QString> > vector;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json);
    QJsonObject jsonObj = jsonDoc.object();
    if (jsonObj.contains(tr("error_msg")))
    {
        ok = false;
        vector.append(qMakePair(QString("error_code"), jsonObj["error_code"].toString()));
        vector.append(qMakePair(QString("error_msg"), jsonObj["error_msg"].toString()));
        vector.append(qMakePair(QString("from"), jsonObj["from"].toString()));
        vector.append(qMakePair(QString("to"), jsonObj["to"].toString()));
        vector.append(qMakePair(QString("query"), jsonObj["query"].toString()));
    }
    else if (jsonObj.contains(tr("trans_result")))
    {
        ok = true;
        vector.append(qMakePair(QString("from"), jsonObj["from"].toString()));
        vector.append(qMakePair(QString("to"), jsonObj["to"].toString()));
        QJsonArray jsonArray = jsonObj["trans_result"].toArray();
        for (auto iter = jsonArray.constBegin(); iter != jsonArray.constEnd(); ++iter)
        {
            QJsonObject jsonObjTmp = static_cast<QJsonValue>(*iter).toObject();
            if (!jsonObjTmp.isEmpty())
                vector.append(qMakePair(jsonObjTmp["src"].toString(), jsonObjTmp["dst"].toString()));
        }
    }
    else
    {
        // unkonwn
        ok = false;
    }

    return vector;
}




