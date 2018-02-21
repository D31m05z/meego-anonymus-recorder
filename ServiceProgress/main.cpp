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

#include <QtGui/QApplication>

#include <MComponentData>
#include <MNotification>
#include <QDebug>

#include "serviceoff.h"

#include <iostream>
#include <stdio.h>
#include <gconfitem.h>
#include <QProcess>
#include <QThread>
#include <QDateTime>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication *app = new QApplication(argc, argv);

    //---------------- notify -----------------
    MComponentData::createInstance(argc, argv);
    MNotification notification(MNotification::DeviceEvent, "", QObject::tr("uploading/progress ..."));
    notification.setImage("icon-m-user-guide");//icon-m-common-done icon-m-user-guide.png
    notification.publish();
    //-----------------------------------------

    ServiceOff service;
    service.vibrate();
    return 0;
    //app->exec();
}
