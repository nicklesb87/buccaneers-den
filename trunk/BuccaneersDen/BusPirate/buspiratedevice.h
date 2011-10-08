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
#ifndef BUSPIRATEDEVICE_H
#define BUSPIRATEDEVICE_H

#include <QObject>

class AbstractSerial;
class Pic24MemoryImage;

class BusPirateDevice : public QObject
{
Q_OBJECT

public:
    explicit BusPirateDevice(QObject *parent = 0);
    virtual ~BusPirateDevice();

    bool ReadVersion();
    QString GetHardwareVersion();
    QString GetFirmwareVersion();
    int GetFirmwareRevision();
    double GetBootloaderVersion();

    int UpdateFirmware(const QString &FirmwareFile);

    bool ConnectDevice(const QString &DeviceName);
    void DisconnectDevice();
    bool IsConnected();

signals:
    void NewData(QByteArray Data);
    void FirmwareUpgradeNbStep(int Maximum);
    void FirmwareUpgradeStep(int Step);

public slots:
    void WritePort(QByteArray Command);

private slots:
    void ReadPort(void);
    void SetMaximumUpgradeStep(int Maximum);
    void SetUpgradeStep(int Step);

private:
    bool IsHiZ();
    bool ResetBusPirate();
    bool EnterBootloader();
    bool OpenPort();
    void ClosePort();
    void ReadHardwareVersion(const QString &line);
    void ReadFirmwareVersion(const QString &line);
    void ReadFirmwareRevision(const QString &line);
    void ReadBootloaderVersion(const QString &line);
    Pic24MemoryImage *GetPicVersion();

    AbstractSerial *m_SerialPort;
    volatile bool m_ForwardData;
    bool m_Connected;
    QString m_BusPirateVersion;
    QString m_FirmwareVersion;
    int m_FirmwareRevision;
    double m_BootloaderVersion;
};


inline QString BusPirateDevice::GetHardwareVersion()
{
    return m_BusPirateVersion;
}

inline QString BusPirateDevice::GetFirmwareVersion()
{
    return m_FirmwareVersion;
}

inline int BusPirateDevice::GetFirmwareRevision()
{
    return m_FirmwareRevision;
}

inline double BusPirateDevice::GetBootloaderVersion()
{
    return m_BootloaderVersion;
}

inline bool BusPirateDevice::IsConnected()
{
    return m_Connected;
}

#endif // BUSPIRATEDEVICE_H
