#-------------------------------------------------
#
# Project created by QtCreator 2013-01-21T17:23:24
#
#-------------------------------------------------

TARGET = ServiceProgress

CONFIG += mobility meegotouch link_pkgconfig
PKGCONFIG += gq-gconf

TEMPLATE = app

MOBILITY = multimedia feedback messaging

QT     += gui network

SOURCES += main.cpp \
    serviceoff.cpp

contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/ServiceProgress/bin
    INSTALLS += target
}

HEADERS += \
    serviceoff.h
