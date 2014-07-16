#-------------------------------------------------
#
# Project created by QtCreator 2014-07-14T16:43:25
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyTranslate
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    cbaidutranslater.cpp

HEADERS  += mainwindow.h \
    cbaidutranslater.h

FORMS    += mainwindow.ui

TRANSLATIONS += zh.ts

OTHER_FILES += \
    .gitignore \
    zh.ts \
    style.css

RESOURCES += \
    resource.qrc
