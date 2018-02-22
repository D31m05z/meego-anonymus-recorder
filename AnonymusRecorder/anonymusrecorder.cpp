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

#include "anonymusrecorder.h"
#include <qaudiocapturesource.h>
#include <iostream>
#include <QDebug>
#include <QTimer>
#include <QProcess>

#include<stdlib.h>

using namespace std;

AnonymusRecorder::AnonymusRecorder(QObject *parent)
    : QObject(parent)
    , m_saveFolder("AnonymRecorder/")
    , m_audioInput(0)
    , m_file(0)
    , m_audiosource(0)
    , m_audioSettings(0)
    , m_capture(0)
    , m_input(0)
    , m_rumble(0)
    , m_source(0)
    , m_camera(0)
    , m_mediaRecorder(0)
    , m_imageCapture(0)
{
    std::cout << "Set the root direction > MyDocs"+m_saveFolder.toStdString() << std::endl;
    QDir dir(QDir::homePath() + QString("/MyDocs/"+m_saveFolder) );

    if(!dir.exists()) {
        std::cout << "Create MyDocs/"+m_saveFolder.toStdString() << std::endl;
        dir.mkdir(QDir::homePath() + QString("/MyDocs/"+m_saveFolder));
    }

    std::cout << "FORMAT" << std::endl;
    QAudioFormat format;
    format.setChannels(1);
    format.setSampleSize(16);
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setCodec("audio/pcm");
    m_format = format;

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
    if (!info.isFormatSupported(format)) {
        std::cout << "Default format not supported - trying to use nearest" << std::endl;
        m_format = info.nearestFormat(format);
    }

    m_audioInput = new QAudioInput(QAudioDeviceInfo::defaultInputDevice(), m_format);
    m_audiosource = new QAudioCaptureSource();
    m_capture = new QMediaRecorder(m_audiosource);
    m_audioSettings = new QAudioEncoderSettings();

    m_rumble = new QFeedbackHapticsEffect();
    m_rumble->setAttackIntensity(0.0);
    m_rumble->setAttackTime(100);
    m_rumble->setIntensity(0.6);
    m_rumble->setDuration(300);
    m_rumble->setFadeTime(100);
    m_rumble->setFadeIntensity(0.+0);
}

AnonymusRecorder::~AnonymusRecorder()
{
    std::cout << "DTOR" << std::endl;
    clean();
    std::cout << "DTOR finished" << std::endl;
}

void AnonymusRecorder::clean()
{
    std::cout << "clean" << std::endl;

    if(m_audioInput)
        delete m_audioInput;

    if(m_file)
        delete m_file;

    if(m_audiosource)
        delete m_audiosource;

    if(m_audioSettings)
        delete m_audioSettings;

    if(m_capture)
        delete m_capture;

    if(m_input)
        delete m_input;

    if(m_rumble)
        delete m_rumble;

    if(m_mediaRecorder)
        delete m_mediaRecorder;

    if(m_camera)
        delete m_camera;

    std::cout << "clean finished" << std::endl;
}

void AnonymusRecorder::stopService()
{
    std::cout << "stopRecording" << std::endl;
    m_audioInput->stop();

    if(m_codec == "wav") {
        std::cout << "CLOSE WAV" << std::endl;
        m_file->close();
    } else {
        std::cout << "STOP CAPTURE" << std::endl;
        m_capture->stop();
    }

    if(m_mediaRecorder) {
        std::cout << "STOP VIDEO RECORDING" << std::endl;
        m_mediaRecorder->stop();
    }
}

void AnonymusRecorder::killService()
{
    std::cout << "shut down service" << std::endl;
    stopService();

    std::cout << "start stop service" << std::endl;
    QProcess process;
    process.execute("/opt/AnonymusRecorder/base/ServiceOff");

    std::cout << "started stop service" << std::endl;
    std::cout << "ICON UPDATE" << std::endl;

    system("cat /opt/AnonymusRecorder/base/off.desktop >/usr/share/applications/AnonymusRecorder.desktop");

    std::cout << "ANONYM EXITING..." << std::endl;

    //TODO: clean
    //clean();
    //destroyed();

    exit(0);
}

void AnonymusRecorder::start(int encoding, int samplerate, int bitrates, bool gps, bool enableCamera )
{
    std::cout << "startRecording" << std::endl;

    switch(encoding) {
    case 1:
        m_codec = "wav";
        m_container = ".wav";
        std::cout << "WAV" << std::endl;
        break;
    case 2:
        m_codec = "audio/AAC";
        m_container = ".aac";
        std::cout << "AAC" << std::endl;
        break;
    case 3:
        m_codec = "audio/FLAC";
        m_container = ".flac";
        std::cout << "FLAC" << std::endl;
        break;
    case 4:
        m_codec = "audio/speex";
        m_container = ".spx";
        std::cout << "SPX" << std::endl;
        break;
    }

    switch(samplerate) {
    case 1:
        std::cout << "22050" << std::endl;
        m_sampleRate = 22050;
        break;
    case 2:
        std::cout << "44100" << std::endl;
        m_sampleRate = 44100;
        break;
    }

    switch(bitrates) {
    case 1:
        std::cout << "32000" << std::endl;
        m_bitRate = 32000;
        break;
    case 2:
        std::cout << "64000" << std::endl;
        m_bitRate = 64000;
        break;
    case 3:
        std::cout << "96000" << std::endl;
        m_bitRate = 96000;
        break;
    case 4:
        std::cout << "128000" << std::endl;
        m_bitRate = 128000;
        break;
    }

    QUrl recordFileUrl = generateAudioFilePath();
    QString name = recordFileUrl.toString();

    QFile file("/home/user/MyDocs/AnonymRecorder/stat.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << name + "*";

    if(m_codec == "wav") {
        // Starts the recording
        m_file = new WavPcmFile(name + m_container, m_audioInput->format());

        if(m_file->open()) {
            std::cout << "START record to WAV" << std::endl;
            m_audioInput->start(m_file);
        } else {
            std::cout << "ERROR" << std::endl;
        }
    } else {
        std::cout << "START record to aac/flac/spx" << std::endl;

        if (m_capture->state() == QMediaRecorder::StoppedState) {
            m_capture->setOutputLocation(QUrl(recordFileUrl.toString()+m_container));

            m_audioSettings->setCodec(m_codec);
            m_audioSettings->setSampleRate(m_sampleRate);
            m_audioSettings->setBitRate(m_bitRate);
            m_audioSettings->setChannelCount(1);
            m_audioSettings->setEncodingMode(QtMultimediaKit::ConstantBitRateEncoding);

            m_capture->setEncodingSettings(*m_audioSettings);
            m_capture->record();
            std::cout << "recording to" << m_capture->outputLocation().toString().toStdString() << std::endl;
        }
    }

    //---gps----
    if(gps) {
        std::cout << "GPS" << std::endl;
        std::cout << "GPS to" << name.toStdString() + ".txt" << std::endl;

        m_source = new logGPS(name +".txt" ,this);
        m_source->enable();
        // out << name + ".txt" << " ";
    }

    if(enableCamera) {
        // TODO
    }
    file.close();
}

void AnonymusRecorder::processSavedImage(int requestId, QString str)
{
    std::cout << "processSavedImage: " << requestId << "str: " << str.toStdString() << endl;
}

void AnonymusRecorder::captureImage()
{
    std::cout << "captureImage" << std::endl;
    if (m_imageCapture->isReadyForCapture()) {
        std::cout << "isReadyForCapture" << std::endl;
        m_imageCapture->capture(QString("/home/user/MyDocs/camera_capture.jpg"));
    } else {
        std::cout << "ERROR: camera is not ready for capture" << std::endl;
    }

}
void AnonymusRecorder::imageCaptured(const QString &fileName, const QImage & /*preview*/)
{
    std::cout << "imageCaptured: " <<  fileName.toStdString() << std::endl;
}

QUrl AnonymusRecorder::generateAudioFilePath()
{
    std::cout << "generateAudioFileName" << std::endl;

    QDateTime dateTime;
    int year=dateTime.currentDateTime().date().year();
    int month=dateTime.currentDateTime().date().month();
    int day=dateTime.currentDateTime().date().day();

    QDir outputDir = QDir::homePath() + QString("/MyDocs/"+m_saveFolder);

    int lastImage = 0;
    int fileCount = 0;

    foreach(QString fileName, outputDir.entryList(QStringList() << "*")) {
        if (outputDir.exists(fileName)){
            std::cout << "$EXIST$ !!! :" << fileName.toStdString() << std::endl;
            fileCount += 1;
            std::cout << "fileCount :" << fileCount << std::endl;
        }
    }

    lastImage += fileCount;
    QUrl location(QDir::toNativeSeparators(outputDir.canonicalPath() + QString("/%1_%2_%3-%4%5").arg(year).arg(month).arg(day).arg(lastImage + 1, 4, 10, QLatin1Char('0')).arg("")));

    bool exist = true;
    int inc = 1;
    while(exist) {
        if (outputDir.exists(location.toString()+m_container)) {
            std::cout << "!! - WARNING: this file is exist alredy!:" << location.toString().toStdString() << std::endl;

            m_note = QString("%1_%2_%3-%4").arg(year).arg(month).arg(day).arg(lastImage + 1 + inc, 4, 10, QLatin1Char('0'));
            QUrl location_tmp(QDir::toNativeSeparators(outputDir.canonicalPath() +"/"+m_note.toString()));

            location = location_tmp;
            inc += 1;
            std::cout << "inc:" << inc << std::endl;

        } else exist = false;
    }

    std::cout << "------------------FINISH_SELECT_FLE_NAME--------------------------------" << std::endl;
    std::cout << "fileCount: " << fileCount<< std::endl;
    std::cout << "lastImage: " << lastImage<< std::endl;
    std::cout << location.toString().toStdString()<< std::endl;
    std::cout << "------------------FINISH_SELECT_FLE_NAME--------------------------------" << std::endl;

    return location;
}

void AnonymusRecorder::vibrate()
{
    std::cout << "vibrate" << std::endl;
    m_rumble->start();
}
