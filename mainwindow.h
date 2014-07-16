#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QVector>
#include <QPair>
#include <QMap>
#include <QMainWindow>
class CBaiduTranslater;
class QComboBox;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void showResult(QVector<QPair<QString, QString> > vector);

private:
    Ui::MainWindow *ui;
    CBaiduTranslater *m_baiduTranslater;
    QString m_from, m_to;       // translate direction

    void initComboBox(QComboBox *comboBox);
};

#endif // MAINWINDOW_H
