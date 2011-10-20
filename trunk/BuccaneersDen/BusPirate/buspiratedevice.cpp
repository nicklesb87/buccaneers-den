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
#include "buspiratedevice.h"
#include <serialport.h>
#include "../lib/hexfileparser.h"
#include "../lib/picmemoryimage.h"
#include "../lib/ds30bootloader.h"

#include <QtGui>

BusPirateDevice::BusPirateDevice(QObject *parent) :
        QObject(parent),
        m_ForwardData(true),
        m_Connected(false)
{

}

BusPirateDevice::~BusPirateDevice()
{
    ClosePort();
}

bool BusPirateDevice::ConnectDevice(const QString &DeviceName)
{
    m_SerialPort = new SerialPort(DeviceName, this);
    if (!m_SerialPort)
        qDebug() << "New SerialPort failled";
    connect(m_SerialPort, SIGNAL(readyRead()), this, SLOT(ReadPort()));
    return OpenPort();
}

void BusPirateDevice::DisconnectDevice()
{
    ClosePort();
}

void BusPirateDevice::ClosePort()
{
    if (m_Connected) {
        m_SerialPort->close();
        m_Connected = false;
    }
}

bool BusPirateDevice::OpenPort()
{
    if (!m_SerialPort->open(SerialPort::ReadWrite)) {
        qDebug() << "Serial device by default: " << m_SerialPort->portName() << " open fail: " << m_SerialPort->errorString();
        m_Connected = false;
    } else {
        m_Connected = true;
    }

    QSettings settings;
    settings.beginGroup("BusPirate");
    int configValue = settings.value("BaudRate", SerialPort::Rate115200).toInt();
    if (!m_SerialPort->setRate((SerialPort::Rate)configValue)) {
        qDebug() << "Set baud rate " <<  configValue << " error.";
    }
    if (!m_SerialPort->setDataBits(SerialPort::Data8)) {
        qDebug() << "Set data bits " <<  configValue << " error.";
    }
    if (!m_SerialPort->setParity(SerialPort::NoParity)) {
        qDebug() << "Set parity " <<  configValue << " error.";
    }
    if (!m_SerialPort->setStopBits(SerialPort::OneStop)) {
        qDebug() << "Set stop bits " <<  configValue << " error.";
    }
    if (!m_SerialPort->setFlowControl(SerialPort::NoFlowControl)) {
        qDebug() << "Set flow " <<  configValue << " error.";
    }
    return m_Connected;
}

void BusPirateDevice::ReadPort()
{
    if (!m_ForwardData)
        return;

    QByteArray byteArray = m_SerialPort->readAll();
    emit NewData(byteArray);
}

void BusPirateDevice::WritePort(QByteArray Data)
{
    m_SerialPort->write(Data);
    m_SerialPort->flush();
}

bool BusPirateDevice::IsHiZ()
{
    QByteArray byteArray("\n");
    m_SerialPort->write(byteArray);
    m_SerialPort->flush();
    while (m_SerialPort->waitForReadyRead(250)) {
        byteArray += m_SerialPort->readAll();
    }
    return byteArray.contains("HiZ>");
}

bool BusPirateDevice::ReadVersion()
{
    disconnect(m_SerialPort, SIGNAL(readyRead()), this, SLOT(ReadPort()));
    m_ForwardData = false;

    if (!IsHiZ())
        ResetBusPirate();

    QByteArray byteArray("i\n");
    m_SerialPort->write(byteArray);
    m_SerialPort->flush();
    while (m_SerialPort->waitForReadyRead(20)) {
        byteArray += m_SerialPort->readAll();
    }

    QString versionInfo(byteArray);
    QStringList infos = versionInfo.split("\n");
    QStringList::const_iterator constIterator;
    for (constIterator = infos.constBegin(); constIterator != infos.constEnd(); ++constIterator) {
        ReadHardwareVersion(*constIterator);
        ReadFirmwareVersion(*constIterator);
        ReadFirmwareRevision(*constIterator);
        ReadBootloaderVersion(*constIterator);
    }

    /* Work around bug in qserialdevice */
    ClosePort();
    OpenPort();

    connect(m_SerialPort, SIGNAL(readyRead()), this, SLOT(ReadPort()));
    m_ForwardData = true;
    return true;
}

void BusPirateDevice::ReadHardwareVersion(const QString &line)
{
    QRegExp BusPirateRevision("Bus Pirate (\\S*)");
    if (-1 != BusPirateRevision.indexIn(line)) {
        m_BusPirateVersion = BusPirateRevision.cap(1);
    }
}

void BusPirateDevice::ReadFirmwareVersion(const QString &line)
{
    QRegExp FirmwareVersion("Firmware v(\\d+\\.\\d*)");
    if (-1 != FirmwareVersion.indexIn(line)) {
        m_FirmwareVersion = FirmwareVersion.cap(1);
    }
}

void BusPirateDevice::ReadFirmwareRevision(const QString &line)
{
    QRegExp FirmwareRevision("\\(r([0-9]+)\\)");
    if (-1 != FirmwareRevision.indexIn(line)) {
        QString capture = FirmwareRevision.cap(1);
        m_FirmwareRevision = capture.toDouble();
    }
}

void BusPirateDevice::ReadBootloaderVersion(const QString &line)
{
    QRegExp BootloaderRevision("Bootloader v(\\d\\.\\d)");
    if (-1 != BootloaderRevision.indexIn(line)) {
        QString capture = BootloaderRevision.cap(1);
        m_BootloaderVersion = capture.toDouble();
    }
}

bool BusPirateDevice::ResetBusPirate()
{

    return true;
}

int BusPirateDevice::UpdateFirmware(const QString &FirmwareFile)
{
    disconnect(m_SerialPort, SIGNAL(readyRead()), this, SLOT(ReadPort()));
    m_ForwardData = false;

    if (!EnterBootloader()) {
        return -1;
    }

    Pic24MemoryImage *Pic = GetPicVersion();
    if (!Pic) {
        QMessageBox::critical(dynamic_cast<QWidget *>(parent()), tr("Unknow Hardware Version"), tr("Sorry we are unable to determine your BusPirate hardware version."));
        return -1;
    }

    Pic->InitializeFromHexFile(FirmwareFile);
    DS30BootLoader bootLoader(m_SerialPort);
    connect(&bootLoader, SIGNAL(FirmwareUpgradeNbStep(int)), this, SLOT(SetMaximumUpgradeStep(int)));
    connect(&bootLoader,SIGNAL(FirmwareUpgradeStep(int)), this, SLOT(SetUpgradeStep(int)));
    if (!bootLoader.SendFirmware(Pic)){
        QMessageBox::critical(dynamic_cast<QWidget *>(parent()), bootLoader.GetErrorTitle(), bootLoader.GetErrorMessage());
        return -1;
    }
    delete Pic;

    ClosePort();
    QMessageBox::information(dynamic_cast<QWidget *>(parent()), tr("Firmware Update Successfull"), tr("The firmware update was successfull. Please disconnect and reconnect your BusPirate before continuing."));
    OpenPort();

    m_ForwardData = true;
    connect(m_SerialPort, SIGNAL(readyRead()), this, SLOT(ReadPort()));
    return 0;
}

Pic24MemoryImage *BusPirateDevice::GetPicVersion()
{
    QRegExp rx("^v(\\d)");
    if (rx.indexIn(m_BusPirateVersion, 0) == -1)
        return 0;

    QString Version = rx.cap(1);
    if (0 == Version.compare("3"))
        return new Pic24fj64MemoryImage;
    else if (0 == Version.compare("4"))
        return new Pic24fj256MemoryImage;

    return 0;
}

bool BusPirateDevice::EnterBootloader()
{
    if (!IsHiZ()) {
        QMessageBox::critical(dynamic_cast<QWidget *>(parent()), tr("Firmware Update"), tr("Please put your BusPirate in HiZ mode before attempting a firmware update."));
        return false;
    }
    QByteArray EnterBootloader("$\n");
    m_SerialPort->write(EnterBootloader);
    m_SerialPort->flush();
    QByteArray Response;
    while (m_SerialPort->waitForReadyRead(50)) {
        Response += m_SerialPort->readAll();
    }

    QString Question(Response);
    QRegExp rx("Are you sure\\?");
    if (rx.indexIn(Question, 0) != -1) {
        QByteArray yesAnswers("y");
        m_SerialPort->write(yesAnswers);
        m_SerialPort->flush();
        Response.clear();
        while (m_SerialPort->waitForReadyRead(50)) {
            Response += m_SerialPort->readAll();
        }
        Question = Response;
    }
    QRegExp BLVersion("BOOTLOADER");
    if (BLVersion.indexIn(Question,0) == -1) {
        QMessageBox::critical(dynamic_cast<QWidget *>(parent()), tr("Firmware Update"), tr("Unknown bootloader version: ").append(Question));
        return false;
    }
    return true;
}

void BusPirateDevice::SetMaximumUpgradeStep(int Maximum)
{
    emit FirmwareUpgradeNbStep(Maximum);
}

void BusPirateDevice::SetUpgradeStep(int Step)
{
    emit FirmwareUpgradeStep(Step);
}
