
MOBILITY = multimedia feedback messaging location
CONFIG += console mobility meegotouch link_pkgconfig
QT     += core gui
PKGCONFIG += gq-gconf

INCLUDEPATH += messaging

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    wav.cpp \
    anonymusrecorder.cpp \
    logGPS.cpp

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog

HEADERS += wav.h \
    anonymusrecorder.h \
    logGPS.h

contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/AnonymusRecorder/bin
    INSTALLS += target
}

contains(MEEGO_EDITION,harmattan) {
    desktopfile.files = AnonymusRecorder.desktop
    desktopfile.path = /usr/share/applications
    INSTALLS += desktopfile
}

contains(MEEGO_EDITION,harmattan) {
    base.files = base/
    base.path = /opt/AnonymusRecorder/
    INSTALLS += base
}

on-icon.path = /usr/share/icons/hicolor/80x80/apps/
on-icon.files = off-anonym.png
INSTALLS += on-icon

off-icon.path = /usr/share/icons/hicolor/80x80/apps/
off-icon.files = on-anonym.png
INSTALLS += off-icon

uploading-icon.path = /usr/share/icons/hicolor/80x80/apps/
uploading-icon.files = uploading-anonym.png
INSTALLS += uploading-icon

pass-icon.path = /usr/share/icons/hicolor/80x80/apps/
pass-icon.files = pass-anonym.png
INSTALLS += pass-icon

settingsdesktop.path = /usr/share/duicontrolpanel/desktops
settingsdesktop.files = SettingsApplet/anonymusrecorder.desktop

settingsxml.path = /usr/share/duicontrolpanel/uidescriptions
settingsxml.files = SettingsApplet/anonymusrecorder.xml

INSTALLS += settingsdesktop settingsxml

