/** * * * Gergely Boross - 2012_02_27 * * * * * * * * * * * * *\
*    _ _____   _____        __ _                              *
*   (_)  __ \ / ____|      / _| |                             *
*   |_| |__)| (___    ___ | |_| |___      ____ _ _ __ ___     *
*   | |  _  / \___ \ / _ \|  _| __\ \ /\ / / _` | '__/ _ \    *
*   | | | \ \ ____) | (_) | | | |_ \ V  V / (_| | | |  __/    *
*   |_|_|  \_\_____/ \___/|_|  \__| \_/\_/ \__,_|_|  \___|    *
*                                                             *
*                http://irsoftware.net                        *
*                                                             *
*              contact_adress: sk8Geri@gmail.com               *
*                                                               *
*       This file is a part of the work done by aFagylaltos.     *
*         You are free to use the code in any way you like,      *
*         modified, unmodified or copied into your own work.     *
*        However, I would like you to consider the following:    *
*                                                               *
*  -If you use this file and its contents unmodified,         *
*              or use a major part of this file,               *
*     please credit the author and leave this note untouched.   *
*  -If you want to use anything in this file commercially,      *
*                please request my approval.                    *
*                                                              *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <QtCore>
#include <iostream>

#include "logGPS.h"

logGPS::logGPS(QString name, QObject *parent) : QObject(parent)
{
    logFile = new QFile();
    logFile->setFileName(name);

    if (!logFile->open(QIODevice::WriteOnly | QIODevice::Text)) {
        std::cout << "Error: cannot create log file" << logFile->fileName().toStdString() << std::endl;
    }

    source = QGeoPositionInfoSource::createDefaultSource(this);
    if (source) {
        std::cout << "source was create" << std::endl;
        source->setUpdateInterval(1000); // time in milliseconds
        source->setPreferredPositioningMethods(QGeoPositionInfoSource::AllPositioningMethods);
    }
}

logGPS::~logGPS()
{
    disable();
    delete logFile;
    delete source;
}

void logGPS::enable()
{
    std::cout << "ENABLED GPS WATCHER" << std::endl;

    if (source) {
        std::cout << "CONNECT" << std::endl;
        connect(source, SIGNAL(positionUpdated(QGeoPositionInfo)),
                this, SLOT(positionUpdated(QGeoPositionInfo)));
        source->startUpdates();
    }
}

void logGPS::disable()
{
    std::cout << "DISABLED GPS WATCHER" << std::endl;

    if (source) {
        source->stopUpdates();
        disconnect(source, SIGNAL(positionUpdated(QGeoPositionInfo)),
                   this, SLOT(positionUpdated(QGeoPositionInfo)));
    }
}

void logGPS::positionUpdated(const QGeoPositionInfo &info)
{
    std::cout << "update"<< std::endl;

    if (info.isValid()) {
        std::cout << "positionUpdated :" <<  info.coordinate().latitude() << std::endl;
        logFile->write("logging: \n");
        qDebug() << info;
    }
}
