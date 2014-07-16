#ifndef CBAIDUTRANSLATER_H
#define CBAIDUTRANSLATER_H

#include <QString>
#include <QObject>
#include <QMap>
#include <QVector>
#include <QPair>
class QNetworkAccessManager;

class CBaiduTranslater: public QObject
{
    Q_OBJECT
public:
    CBaiduTranslater(QString API_Key = "",
                     QString url = "http://openapi.baidu.com/public/2.0/bmt/translate",
                     QObject *parent = 0);

    QString API_Key() const;
    void setAPI_Key(const QString &API_Key);

    QString from() const;
    void setFrom(const QString &from);

    QString to() const;
    void setTo(const QString &to);

    QString q() const;
    void setQ(const QString &q);

    void translate();
    void translate(const QString &src);
    void translate(const QString &src, const QString from, const QString to);

signals:
    void error(QMap<QString, QString>);
    void finished(QVector<QPair<QString, QString> >);

private:
    QString m_url;
    QString m_API_Key;
    QString m_from;         // source lauguage
    QString m_to;           // destination lauguage
    QString m_client_id;    // baidu API key
    QString m_q;            // query content

    QNetworkAccessManager *m_networkAccessManager;

    QVector<QPair<QString, QString> > decodeJsonData(const QByteArray &json, bool &ok);
};

/*
中文 	zh 	    英语 	en
日语 	jp 	    韩语 	kor
西班牙语 	spa 	法语 	fra
泰语 	th 	    阿拉伯语 	ara
俄罗斯语 	ru 	    葡萄牙语 	pt
粤语 	yue 	文言文 	wyw
白话文 	zh 	    自动检测 	auto
*/

#endif // CBAIDUTRANSLATER_H
