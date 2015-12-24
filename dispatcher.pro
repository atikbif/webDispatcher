#-------------------------------------------------
#
# Project created by QtCreator 2015-11-25T11:08:36
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += serialport
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

RC_ICONS = web.ico
VERSION = 1.0.0.1
QMAKE_TARGET_COMPANY = Kontel Co
QMAKE_TARGET_PRODUCT = WebDispatcher
QMAKE_TARGET_DESCRIPTION = Web Server
QMAKE_TARGET_COPYRIGHT = Kontel

TARGET = dispatcher
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qhttpserver-master/http-parser/http_parser.c \
    qhttpserver-master/src/qhttpconnection.cpp \
    qhttpserver-master/src/qhttprequest.cpp \
    qhttpserver-master/src/qhttpresponse.cpp \
    qhttpserver-master/src/qhttpserver.cpp \
    server/servermanager.cpp \
    server/server.cpp \
    server/responder.cpp \
    link/controllerdata.cpp \
    link/objectdata.cpp \
    link/objlist.cpp \
    dynamic/dynreqmanager.cpp \
    webPages/pagecreator.cpp \
    dynamic/datastorage.cpp \
    dynamic/analogdatavar.cpp \
    dynamic/objectvars.cpp \
    dynamic/discretedatavar.cpp \
    dynamic/messagedatavar.cpp \
    link/linkmanager.cpp \
    link/objectmanager.cpp \
    link/linkobject.cpp \
    Protocols/asciidecorator.cpp \
    Protocols/checksum.cpp \
    Protocols/commanddecorator.cpp \
    Protocols/commandinterface.cpp \
    Protocols/request.cpp \
    Protocols/rk.cpp \
    Protocols/rkcommand.cpp \
    Protocols/udpdecorator.cpp \
    Protocols/tcpdecorator.cpp

HEADERS  += mainwindow.h \
    qhttpserver-master/http-parser/http_parser.h \
    qhttpserver-master/src/qhttpconnection.h \
    qhttpserver-master/src/qhttprequest.h \
    qhttpserver-master/src/qhttpresponse.h \
    qhttpserver-master/src/qhttpserver.h \
    qhttpserver-master/src/qhttpserverfwd.h \
    server/servermanager.h \
    server/server.h \
    server/responder.h \
    link/controllerdata.h \
    link/objectdata.h \
    link/variables.h \
    link/objlist.h \
    dynamic/dynreqmanager.h \
    webPages/pagecreator.h \
    dynamic/datastorage.h \
    dynamic/analogdatavar.h \
    dynamic/objectvars.h \
    dynamic/discretedatavar.h \
    dynamic/messagedatavar.h \
    link/linkmanager.h \
    link/objectmanager.h \
    link/linkobject.h \
    Protocols/asciidecorator.h \
    Protocols/checksum.h \
    Protocols/commanddecorator.h \
    Protocols/commandinterface.h \
    Protocols/request.h \
    Protocols/rk.h \
    Protocols/rkcommand.h \
    Protocols/udpdecorator.h \
    Protocols/tcpdecorator.h

FORMS    += mainwindow.ui

RESOURCES += \
    webpatterns.qrc

DISTFILES += \
    variables.qmodel
