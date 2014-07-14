#include <QtCore>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cbaidutranslater.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_baiduTranslater(new CBaiduTranslater)
{
    ui->setupUi(this);

    setWindowTitle(tr("MyTranslater developed with Qt"));

    // Set baidu translate seveice API key
    m_baiduTranslater->setAPI_Key("MMy04nuApNDEaIQGSV6XncBv");

    /* Chinese to English */
    connect(ui->btnTranslateAB, &QPushButton::clicked, [=]{
        QString srcText = ui->textA->toPlainText();
        m_baiduTranslater->translate(srcText, "zh", "en");
    });

    /* English to Chinese  */
    connect(ui->btnTranslateBA, &QPushButton::clicked, [=]{
        QString srcText = ui->textB->toPlainText();
        m_baiduTranslater->translate(srcText, "en", "zh");
    });

    connect(m_baiduTranslater, &CBaiduTranslater::finished, this, &MainWindow::showResult);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showResult(QVector<QPair<QString, QString> >vector)
{
    QString destText;

    QVectorIterator<QPair<QString, QString> > iter(vector);
    QString from = iter.next().second;
    QString to = iter.next().second;
    while(iter.hasNext())
    {
        QPair<QString, QString> pair = iter.next();
        destText += pair.second + "\n";
    }

    if (to == "zh")
        ui->textA->setPlainText(destText);
    else
        ui->textB->setPlainText(destText);
}
