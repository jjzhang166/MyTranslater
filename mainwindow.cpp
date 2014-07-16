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

    /* translate */
    connect(ui->btnTranslate, &QPushButton::clicked, [=]{
        // show wait animation.
        ui->plainTextEdit_dst->clear();
        QString srcText = ui->plainTextEdit_src->toPlainText();
        m_baiduTranslater->translate(srcText, "auto", "auto");      // emit signal "finished" when finish the translation.
    });

//    /* English to Chinese  */
//    connect(ui->btnTranslate, &QPushButton::clicked, [=]{
//        QString srcText = ui->plainTextEdit_dst->toPlainText();
//        m_baiduTranslater->translate(srcText, "auto", "auto");      // emit signal "finished" when finish the translation.
//    });

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

    ui->plainTextEdit_dst->setPlainText(destText);
}
