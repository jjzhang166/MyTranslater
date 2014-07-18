#include <QtCore>
#include <QComboBox>
#include <QToolButton>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cbaidutranslater.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_baiduTranslater(new CBaiduTranslater),
    m_from("auto"), m_to("auto")
{
    ui->setupUi(this);

    /* Stay on top tool button */
    m_showTop = new QToolButton(this);
    m_showTop->setToolTip(tr("stay on top"));
    connect(m_showTop, &QToolButton::clicked, this, &MainWindow::toggleStayOnTop);
    ui->statusBar->addWidget(m_showTop, 0);

    /* Update style tool button */
    m_updateStyle = new QToolButton(this);
    m_updateStyle->setToolTip(tr("update style"));
    ui->statusBar->addWidget(m_updateStyle, 0);
    connect(m_updateStyle, &QToolButton::clicked, this, &MainWindow::updateStyle);

    /* status display label */
    m_statusInfo = new QLabel(this);
    m_statusInfo->setText(tr("The first time query may be slow"));
    ui->statusBar->addWidget(m_statusInfo, 0);


    /* initialize translation direction */
    initComboBox(ui->comboBox);

    setWindowTitle(tr("MyTranslater"));

    // Set baidu translate seveice API key
    m_baiduTranslater->setAPI_Key("MMy04nuApNDEaIQGSV6XncBv");

    /* translate */
    connect(ui->btnTranslate, SIGNAL(clicked()), this, SLOT(translate()));
    connect(m_baiduTranslater, &CBaiduTranslater::finished, this, &MainWindow::showResult);

    /* initialize waiting label */
//    ui->label_statusPicture->hide();
    ui->label_statusPicture->setFileName(":/res/loading.gif");
    ui->label_statusPicture->hide();
    ui->label_translationStatus->setText(tr("Translating..."));
    ui->label_translationStatus->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::translate()
{
    if (ui->plainTextEdit_src->toPlainText().isEmpty())
        return;

    ui->plainTextEdit_dst->clear();
    QString srcText = ui->plainTextEdit_src->toPlainText();
    translate(srcText, m_from, m_to);

    // show waiting animation...
    ui->label_statusPicture->show();
    ui->label_statusPicture->play();
    ui->label_translationStatus->show();
}

void MainWindow::translate(const QString &srcText, const QString &from, const QString &to)
{
    m_baiduTranslater->translate(srcText, from, to);
}

void MainWindow::showResult(CBaiduTranslateResult result)
{
    QString destText;

    if (result.m_error_code != CBaiduTranslateResult::None)
    {
        QString error_msg = "<font color='red'>%1</font>";
        switch (result.m_error_code) {
        case CBaiduTranslateResult::Timeout:
            m_statusInfo->setText(error_msg.arg(tr("time out")));
            break;
        case CBaiduTranslateResult::SystemError:
            m_statusInfo->setText(error_msg.arg(tr("system error")));
            break;
        case CBaiduTranslateResult::UnauthorizedUser:
            m_statusInfo->setText(error_msg.arg(tr("unauthorized user")));
            break;
        default:
            break;
        }
        return;
    }

    QVectorIterator<QPair<QString, QString> > iter(result.m_trans_result);
    while(iter.hasNext())
    {
        QPair<QString, QString> pair = iter.next();
        destText += pair.second + "\n";
    }

    ui->plainTextEdit_dst->setPlainText(destText);

    // hide waiting animation...
    ui->label_statusPicture->stop();
    ui->label_statusPicture->hide();
    ui->label_translationStatus->hide();
}

void MainWindow::toggleStayOnTop()
{
    static bool onTop = true;
    static QPoint pos = this->pos();
    Qt::WindowFlags flags = Qt::WindowStaysOnTopHint;
    if (onTop)
        this->setWindowFlags(this->windowFlags() | flags);
    else
        this->setWindowFlags(this->windowFlags() & ~flags);
    onTop = !onTop;
    this->move(pos);
    this->show();

}

void MainWindow::updateStyle()
{
    QString appDir = QApplication::applicationDirPath() + QDir::separator();
    /* load style sheet. */
    QFile file(appDir + "style/style.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QString style(file.readAll());
//        qDebug() << style;
        qApp->setStyleSheet(style);
        file.close();
    }
}


/*
中文 	zh 	    英语 	en
日语 	jp 	    韩语 	kor
西班牙语 	spa 	    法语 	fra
泰语 	th 	    阿拉伯语 	ara
俄罗斯语 	ru 	    葡萄牙语 	pt
粤语 	yue 	    文言文 	wyw
白话文 	zh 	    自动检测 	auto
*/
void MainWindow::initComboBox(QComboBox *comboBox)
{
    if (comboBox == nullptr)
        return;

    comboBox->addItem(tr("Auto detection"), QStringList() << "auto" << "auto");
    comboBox->addItem(tr("English -> Chinese"), QStringList() << "en" << "zh");
    comboBox->addItem(tr("Chinese -> English"), QStringList() << "zh" << "en");
    comboBox->addItem(tr("Chinese -> Japanese"), QStringList() << "zh" << "jp");
    comboBox->addItem(tr("Chinese -> Korean"), QStringList() << "zh" << "kor");
    comboBox->addItem(tr("BaiHuaWen-> WenYanWen"), QStringList() << "zh" << "wyw");
    comboBox->addItem(tr("WenYanWen -> BaiHuaWen"), QStringList() << "wyw" << "zh");

    auto currentIndexChanged = static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged);
    connect(comboBox, currentIndexChanged, [=](int index){
        QStringList strList = comboBox->itemData(index, Qt::UserRole).toStringList();
        m_from = strList[0];
        m_to = strList[1];
    });
}
