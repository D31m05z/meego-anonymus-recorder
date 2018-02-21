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

#include "wav.h"

WavPcmFile::WavPcmFile(const QString & name, const QAudioFormat & format_, QObject *parent_)
    : QFile(name, parent_), format(format_)
{
}

bool WavPcmFile::hasSupportedFormat()
{
    return (format.sampleSize() == 8
            && format.sampleType() == QAudioFormat::UnSignedInt)
            || (format.sampleSize() > 8
                && format.sampleType() == QAudioFormat::SignedInt
                && format.byteOrder() == QAudioFormat::LittleEndian);
}

bool WavPcmFile::open()
{
    if (!hasSupportedFormat()) {
        setErrorString("Wav PCM supports only 8-bit unsigned samples "
                       "or 16-bit (or more) signed samples (in little endian)");
        return false;
    } else {
        if (!QFile::open(ReadWrite | Truncate))
            return false;
        writeHeader();
        return true;
    }
}

void WavPcmFile::writeHeader()
{
    QDataStream out(this);
    out.setByteOrder(QDataStream::LittleEndian);

    // RIFF chunk
    out.writeRawData("RIFF", 4);
    out << quint32(0); // Placeholder for the RIFF chunk size (filled by close())
    out.writeRawData("WAVE", 4);

    // Format description chunk
    out.writeRawData("fmt ", 4);
    out << quint32(16); // "fmt " chunk size (always 16 for PCM)
    out << quint16(1);  // data format (1 => PCM)
    out << quint16(format.channelCount());
    out << quint32(format.sampleRate());
    out << quint32(format.sampleRate() * format.channelCount()
                   * format.sampleSize() / 8 ); // bytes per second
    out << quint16(format.channelCount() * format.sampleSize() / 8); // Block align
    out << quint16(format.sampleSize()); // Significant Bits Per Sample

    // Data chunk
    out.writeRawData("data", 4);
    out << quint32(0);  // Placeholder for the data chunk size (filled by close())

    Q_ASSERT(pos() == 44); // Must be 44 for WAV PCM
}

void WavPcmFile::close()
{
    // Fill the header size placeholders
    quint32 fileSize = size();

    QDataStream out(this);
    // RIFF chunk size
    seek(4);
    out << quint32(fileSize - 8);

    // data chunk size
    seek(40);
    out << quint32(fileSize - 44);

    QFile::close();
}
