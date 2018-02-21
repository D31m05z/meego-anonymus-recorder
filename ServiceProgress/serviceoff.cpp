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

#include "serviceoff.h"

ServiceOff::ServiceOff()
{

}

void ServiceOff::effectStateChanged()
{
    QFeedbackHapticsEffect *eff = static_cast<QFeedbackHapticsEffect*>(QObject::sender());
    if (eff->state() == QFeedbackEffect::Stopped) {
        std::cout << "effect stopped"<< std::endl;
        qDebug() << "effect stopped";

        eff->deleteLater();
        QApplication::exit();
        std::cout << "exited $$$$$"<< std::endl;
    }
}

void ServiceOff::vibrate()
{
    std::cout << "rezgek"<< std::endl;
    QFeedbackHapticsEffect *rumble = new QFeedbackHapticsEffect();
    rumble->setAttackIntensity(0.0);
    rumble->setAttackTime(100);
    rumble->setIntensity(0.6);
    rumble->setDuration(300);
    rumble->setFadeTime(100);
    rumble->setFadeIntensity(0.+0);

    rumble->start();

    connect(rumble, SIGNAL(stateChanged()), this, SLOT(effectStateChanged()));
}
