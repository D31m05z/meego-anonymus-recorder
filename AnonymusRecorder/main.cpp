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
#include <QObject>
#include <QTimer>

#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <gconfitem.h>

#include "anonymusrecorder.h"

#include<stdlib.h>

AnonymusRecorder* recorder;
QApplication* app;

void term(int signum)
{
    system("cat /opt/AnonymusRecorder/base/off.desktop >/usr/share/applications/AnonymusRecorder.desktop");

    printf("Received SIGTERM, exiting...\n");
    std::cout << "shut down service: " << signum << std::endl;
    recorder->stopService();
    delete recorder;

    app->quit();
}

int main(int argc, char *argv[])
{
    system("cat /opt/AnonymusRecorder/base/on.desktop >/usr/share/applications/AnonymusRecorder.desktop");
    app = new QApplication(argc, argv);

    bool enableNotify = false;
    bool enableVibrate = false;

    GConfItem config_level("/apps/ControlPanel/AnonymusRecorder/Level");
    std::cout << config_level.value().toString().toStdString() << std::endl;
    int level = config_level.value().toInt();

    GConfItem config_autokill("/apps/ControlPanel/AnonymusRecorder/AutoKillEnable");
    std::cout << config_autokill.value().toString().toStdString() << std::endl;
    bool autoKill = config_autokill.value().toInt();

    GConfItem config_autokill_time("/apps/ControlPanel/AnonymusRecorder/AutoKill");
    std::cout << config_autokill_time.value().toString().toStdString() << std::endl;
    int autoKillTimer = config_autokill_time.value().toInt();

    std::cout << "level = " << level << std::endl;
    if(level == 1) {
        enableNotify = false;
        enableVibrate = false;
    } else if(level == 2) {
        enableNotify = false;
        enableVibrate = true;
    } else if(level == 3) {
        enableNotify = true;
        enableVibrate = true;
    } else if(level == 4) {
        enableNotify = true;
        enableVibrate = true;

        if(autoKill)
            system("gconftool-2 -t string -s /desktop/meego/screen_lock/low_power_mode/operator_logo /opt/AnonymusRecorder/base/standby-autokill.png");
        else
            system("gconftool-2 -t string -s /desktop/meego/screen_lock/low_power_mode/operator_logo /opt/AnonymusRecorder/base/standby-recording.png");

        // system("/sbin/initctl restart xsession/sysuid");
    }

    GConfItem config_codec("/apps/ControlPanel/AnonymusRecorder/Quality");
    std::cout << config_codec.value().toString().toStdString() << std::endl;
    int codec = config_codec.value().toInt();

    GConfItem config_samplerate("/apps/ControlPanel/AnonymusRecorder/Sample");
    std::cout << config_samplerate.value().toString().toStdString() << std::endl;
    int samplerate = config_samplerate.value().toInt();

    GConfItem config_bitrates("/apps/ControlPanel/AnonymusRecorder/Bitrate");
    std::cout << config_bitrates.value().toString().toStdString() << std::endl;
    int bitrates = config_bitrates.value().toInt();

    /// TODO
    /* GConfItem config_gps("/apps/ControlPanel/AnonymusRecorder/Gps");
    std::cout << config_gps.value().toString().toStdString() << std::endl;*/
    bool gps = false;//config_gps.value().toInt();

    /// TODO
    // GConfItem config_video("/apps/ControlPanel/AnonymusRecorder/Video");
    // std::cout << config_video.value().toString().toStdString() << std::endl;
    bool enableCamera = false; //config_video.value().toInt();

    if(enableNotify) {
        MComponentData::createInstance(argc, argv);
        QString msg;
        if(autoKill)
            msg = "Recording...\nWARNING autoKill ON! " + QString("%1").arg(autoKillTimer) + "min";
        else
            msg = "Anonymus recording started...";

        MNotification notification(MNotification::DeviceEvent, "", msg);

        notification.setImage("icon-m-user-guide");
        notification.publish();
    }

    AnonymusRecorder recorder(0);
    recorder.start(codec, samplerate, bitrates, gps,enableCamera);

    if(autoKill) {
        std::cout << "AUTO KILL ENABLED : " <<autoKillTimer<< std::endl;
        QTimer::singleShot(autoKillTimer * 60000, &recorder, SLOT(killService()));
    }

    if(enableVibrate) {
        recorder.vibrate();
    }

    std::cout << "start service" << std::endl;
    signal(SIGTERM, term);

    return app->exec();
}

