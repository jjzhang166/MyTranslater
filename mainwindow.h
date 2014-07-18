#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QVector>
#include <QPair>
#include <QMap>
#include <QMainWindow>
#include "entities/cbaidutranslateresult.h"
class CBaiduTranslater;
class QComboBox;
class QLabel;
class QToolButton;
class QMouseEvent;

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
    void translate();
    void translate(const QString &srcText, const QString &from, const QString &to);
    void showResult(CBaiduTranslateResult result);
    void toggleStayOnTop();
    void updateStyle();

private:
    Ui::MainWindow *ui;
    CBaiduTranslater *m_baiduTranslater;
    QString m_from, m_to;       // translate direction
    QLabel *m_statusInfo;
    QToolButton *m_showTop;
    QToolButton *m_updateStyle;

    void initComboBox(QComboBox *comboBox);
};

#endif // MAINWINDOW_H
