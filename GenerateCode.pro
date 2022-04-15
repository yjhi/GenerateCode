#-------------------------------------------------
#
# Project created by QtCreator 2022-04-14T09:06:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GenerateCode
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    models.cpp

HEADERS  += mainwindow.h \
    models.h

FORMS    += mainwindow.ui

RESOURCES += \
    rc.qrc

RC_ICONS =logo.ico

VERSION = 1.0.2.0


QMAKE_TARGET_PRODUCT =GenerateCode
QMAKE_TARGET_COMPANY = yjhi
QMAKE_TARGET_DESCRIPTION = github.com/yjhi/GenerateCode
QMAKE_TARGET_COPYRIGHT = @yjhi
