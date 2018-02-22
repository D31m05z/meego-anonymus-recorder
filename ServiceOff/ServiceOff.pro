#-------------------------------------------------
#
# Project created by QtCreator 2013-01-09T23:16:11
#
#-------------------------------------------------

TARGET = ServiceOff

CONFIG += meegotouch mobility link_pkgconfig
PKGCONFIG += gq-gconf
MOBILITY = multimedia feedback messaging
QT     += gui network

TEMPLATE = app

SOURCES += main.cpp \
    serviceoff.cpp

contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/ServiceOff/bin
    INSTALLS += target
}

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog

HEADERS += \
    serviceoff.h
