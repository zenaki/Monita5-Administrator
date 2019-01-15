#-------------------------------------------------
#
# Project created by QtCreator 2018-10-05T13:07:21
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = monita5-administrator
TEMPLATE = app


SOURCES += main.cpp     \
    view/mainwindow.cpp \
    view/login.cpp \
    configure/config.cpp \
    configure/mysql.cpp \
    view/user_management.cpp \
    view/company_management.cpp \
    view/asset_management.cpp \
    view/module_management.cpp \
    view/childwindow.cpp

HEADERS  +=             \
    view/mainwindow.h   \
    view/login.h \
    configure/config.h \
    configure/mysql.h \
    configure/common.h \
    view/user_management.h \
    view/company_management.h \
    view/asset_management.h \
    view/module_management.h \
    view/childwindow.h

FORMS    +=             \
    view/mainwindow.ui  \
    view/login.ui \
    view/user_management.ui \
    view/company_management.ui \
    view/asset_management.ui \
    view/module_management.ui

MOC_DIR = $$PWD/build/$$QTBUILDTYPE
RCC_DIR = $$PWD/build/$$QTBUILDTYPE
UI_DIR = $$PWD/build/$$QTBUILDTYPE
OBJECTS_DIR = $$PWD/build/$$QTBUILDTYPE
DESTDIR = $$PWD/build/$$QTBUILDTYPE
