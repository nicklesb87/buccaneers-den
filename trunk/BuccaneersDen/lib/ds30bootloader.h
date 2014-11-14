/******************************************************************************
 *
 * This file is part of Buccaneer's Den.
 *
 * Copyright (C) 2011 Vincent Roy http://code.google.com/p/buccaneers-den/
 *
 * Buccaneer's Den is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Buccaneer's Den is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Buccaneer's Den.  If not, see <http://www.gnu.org/licenses/>.
 *
 *****************************************************************************/
#ifndef DS30BOOTLOADER_H
#define DS30BOOTLOADER_H

#include <QtCore>

class QSerialPort;
class Pic24MemoryImage;

class DS30Command : public QByteArray
{
public:
    void PrepareErasePage(quint32 Address);
    void PrepareWriteRow(quint32 Address, const QByteArray &Data, const Pic24MemoryImage *PicDetails);

private:
    void SetAddress(quint32 Address) {
        (*this)[0] = (Address & 0xFF0000) >> 16;
        (*this)[1] = (Address & 0xFF00) >> 8;
        (*this)[2] = (Address & 0xFF);
    }

    quint8 ComputeCRC();
    static const int COMMAND_OFFSET = 3;
    static const int LENGTH_OFFSET = 4;
    static const int PAYLOAD_OFFSET = 5;
    static const int HEADER_LENGTH = PAYLOAD_OFFSET;
    static const quint8 ERASE_COMMAND = 0x01;
    static const quint8 WRITE_COMMAND = 0x02;
};

class DS30BootLoader : public QObject
{
Q_OBJECT

public:
    DS30BootLoader(QSerialPort *SerialPort, QObject *parent = 0);
    bool SendFirmware(const Pic24MemoryImage *PicDetails);
    QString GetErrorTitle();
    QString GetErrorMessage();

signals:
    void FirmwareUpgradeNbStep(int Maximum);
    void FirmwareUpgradeStep(int Step);

private:
    bool IsDeviceReady();
    int SendCommand(const DS30Command &Command);
    QSerialPort *m_SerialPort;
    QString m_ErrorTitle;
    QString m_ErrorMessage;

    static const quint8 HELLO = 0xC1;
    static const quint8 OK = 'K';
    static const quint8 PROTECTION = 'P';
};

inline QString DS30BootLoader::GetErrorTitle()
{
    return m_ErrorTitle;
}

inline QString DS30BootLoader::GetErrorMessage()
{
    return m_ErrorMessage;
}

#endif // DS30BOOTLOADER_H
