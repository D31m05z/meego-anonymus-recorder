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

#ifndef ANONYMUSRECORDER_H
#define ANONYMUSRECORDER_H

#include "wav.h"

#include <QtGui/QApplication>
#include <QDir>
#include <QDebug>

#include <QUrl>
#include <QDateTime>
#include <QFeedbackHapticsEffect>
#include <QObject>
#include <QSocketNotifier>
#include <QCamera>

#include <qmediarecorder.h>
#include <qaudioinput.h>
#include "logGPS.h"
#include <QCameraImageCapture>

using namespace QtMobility;
class QAudioCaptureSource;

class AnonymusRecorder: public QObject
{
    Q_OBJECT
public:
    AnonymusRecorder(QObject *parent);
    ~AnonymusRecorder();

    QUrl generateAudioFilePath();
    void vibrate();
    void start(int encoding, int samplerate, int bitrates, bool gps, bool enableCamera);
    void stopService();
private:
    void clean();

public slots:
    void killService();

    void captureImage();
    void imageCaptured(const QString &fileName, const QImage &preview);
    void processSavedImage(int requestId, QString str);

private:
    QString m_saveFolder;
    QUrl m_note;
    QAudioFormat m_format;
    QString m_codec;
    QString m_container;
    int m_sampleRate;
    int m_bitRate;

    QAudioInput* m_audioInput;
    WavPcmFile* m_file;
    QAudioCaptureSource* m_audiosource;
    QAudioEncoderSettings*  m_audioSettings;
    QMediaRecorder* m_capture;
    QIODevice* m_input;
    QFeedbackHapticsEffect* m_rumble;
    logGPS* m_source;
    QCamera* m_camera;
    QMediaRecorder* m_mediaRecorder;
    QCameraImageCapture* m_imageCapture;
};

#endif // ANONYMUSRECORDER_H
