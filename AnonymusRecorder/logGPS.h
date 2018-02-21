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

#ifndef LOGFILEPOSITIONSOURCE_H
#define LOGFILEPOSITIONSOURCE_H

#include <qgeopositioninfosource.h>
#include <QFile>
#include <QObject>

using namespace QtMobility;

class logGPS: public QObject
{
    Q_OBJECT
public:
    logGPS(QString name, QObject *parent);
    ~logGPS();
    void enable();
    void disable();

private slots:
    void positionUpdated(const QGeoPositionInfo &info);

private:
    QFile *logFile;
    QGeoPositionInfoSource *source;
};

#endif
