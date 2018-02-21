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
#include <QProcess>
#include <QThread>
#include <QDateTime>
#include <QFile>

#include "serviceoff.h"

#ifdef HAS_MAIL_SERVICE
#include "mailService/SmtpMime"
#endif

#include <iostream>
#include <stdio.h>
#include <gconfitem.h>

using namespace std;
void compress(bool security, QString password)
{
    std::cout << "Compressing settings" << std::endl;

    QFile file("/home/user/MyDocs/AnonymRecorder/stat.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString command="tar -jcvf /home/user/MyDocs/AnonymRecorder/data.tar.bz2 ";

    while (!in.atEnd()) {
        QString line = in.readLine();
        command.append(line);
    }

    cout << command.toStdString() << endl;

    std::cout << "COMPRESSING" << std::endl;

    system(command.toStdString().c_str());

    if(security) {
        //echo password | gpg --batch -q -o /tmp/file.tgz --passphrase-fd 0 --decrypt /tmp/file.tgz.gpg
        //echo password | gpg --batch -q --passphrase-fd 0 --cipher-algo AES256 -c /tmp/file.tgz

        std::cout << "security settings" << std::endl;

        command = "echo " + password +" | gpg --batch -q --passphrase-fd 0 -c " + "/home/user/MyDocs/AnonymRecorder/data.tar.bz2";

        std::cout << "Encryption " << std::endl;
        system(command.toStdString().c_str());
    }

    file.close();
}

void deleteTmpFiles(bool security)
{
    std::cout << "deleteTmpFiles" << std::endl;

    QFile file("/home/user/MyDocs/AnonymRecorder/stat.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString command="rm ";

    while (!in.atEnd()) {
        QString line = in.readLine();
        command.append(line);
    }
    system(command.toStdString().c_str());

    if(security) {
        system("rm /home/user/MyDocs/AnonymRecorder/data.tar.bz2");
    }

    file.close();
}

int main(int argc, char *argv[])
{
    system("cat /opt/AnonymusRecorder/base/up.desktop >/usr/share/applications/AnonymusRecorder.desktop");

    QApplication *app = new QApplication(argc, argv);

    bool enableNotify = false;
    bool enableVibrate = false;
    bool enableStandBy = false;

    GConfItem*  config = new GConfItem("/apps/ControlPanel/AnonymusRecorder/Level");
    std::cout << config->value().toString().toStdString() << std::endl;
    int level = config->value().toInt();

    config = new GConfItem("/apps/ControlPanel/AnonymusRecorder/EMail");
    std::cout << config->value().toString().toStdString() << std::endl;
    QString mail_addres = config->value().toString();

    config = new GConfItem("/apps/ControlPanel/AnonymusRecorder/EnableSecurity");
    std::cout << config->value().toString().toStdString() << std::endl;
    bool enableSecurity = config->value().toInt();

    config = new GConfItem("/apps/ControlPanel/AnonymusRecorder/SecurityCode");
    std::cout << config->value().toString().toStdString() << std::endl;
    QString password = config->value().toString();

    config = new GConfItem("/apps/ControlPanel/AnonymusRecorder/AutoRemove");
    std::cout << config->value().toString().toStdString() << std::endl;
    bool enableAutoRemove = config->value().toInt();

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
        enableStandBy = true;

        if(mail_addres != "") {
            system("gconftool-2 -t string -s /desktop/meego/screen_lock/low_power_mode/operator_logo /opt/AnonymusRecorder/base/standby-uploading.png");
            //system("/sbin/initctl restart xsession/sysuid");
        }
    }

    bool result = false;

    if(enableNotify) {
        //---------------- notify -----------------
        MComponentData::createInstance(argc, argv);
        MNotification notification(MNotification::DeviceEvent, "", QObject::tr("Anonymus recording stopped..."));
        notification.setImage("icon-m-user-guide");//icon-m-common-done icon-m-user-guide.png
        notification.publish();
        //-----------------------------------------
    }

    ServiceOff serviceOff;

    if(enableVibrate) {
        serviceOff.vibrate();
    }

    if(mail_addres != "") {
        std::cout << "REMOVE OLD DATA" << std::endl;
        std::cout << "data.tar.bz2" << std::endl;
        system("rm /home/user/MyDocs/AnonymRecorder/data.tar.bz2");
        std::cout << "data.tar.bz2.gpg" << std::endl;
        system("rm /home/user/MyDocs/AnonymRecorder/data.tar.bz2.gpg");

        compress(enableSecurity, password);

#ifdef HAS_MAIL_SERVICE
        result = sendMail(mail_addres, enableSecurity);
#endif

        // if set the e-mail adress and sent the file to your mail box, and if set enableAutoRemove and sucessfully send
        // it will remove all temporaly files
        if(enableAutoRemove) {
            if(result) {
                deleteTmpFiles(enableSecurity);
            }
        }
    }

    for(int i=0;i<4;i++) {
        if(result) {
            system("cat /opt/AnonymusRecorder/base/pass.desktop >/usr/share/applications/AnonymusRecorder.desktop");

            if(enableStandBy) {
                system("gconftool-2 -t string -s /desktop/meego/screen_lock/low_power_mode/operator_logo /opt/AnonymusRecorder/base/standby-successfully.png");
                //  system("/sbin/initctl restart xsession/sysuid");
            }
        } else {
            system("cat /opt/AnonymusRecorder/base/fail.desktop >/usr/share/applications/AnonymusRecorder.desktop");

            if(enableStandBy) {
                system("gconftool-2 -t string -s /desktop/meego/screen_lock/low_power_mode/operator_logo /opt/AnonymusRecorder/base/standby-recording.png");
                //  system("/sbin/initctl restart xsession/sysuid");
            }
        }

        sleep(1);
        system("cat /opt/AnonymusRecorder/base/idle.desktop >/usr/share/applications/AnonymusRecorder.desktop");

        if(enableStandBy) {
            system("gconftool-2 -t string -s /desktop/meego/screen_lock/low_power_mode/operator_logo /opt/AnonymusRecorder/base/standby-idle.png");
            //  system("/sbin/initctl restart xsession/sysuid");
        }
        sleep(1);
    }

    if(enableStandBy) {
        system("cat /opt/AnonymusRecorder/base/off.desktop >/usr/share/applications/AnonymusRecorder.desktop");
    }

    system("gconftool-2 -t string -s /desktop/meego/screen_lock/low_power_mode/operator_logo /opt/AnonymusRecorder/base/standby-nothing.png");
    // system("/sbin/initctl restart xsession/sysuid");

    return 0;//app->exec();
}
