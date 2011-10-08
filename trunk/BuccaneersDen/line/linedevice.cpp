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
#include "linedevice.h"
#include <abstractserial.h>
#include <QtGui>

LineDevice::LineDevice(QWidget *parent) :
    QWidget(parent),
    m_Connected(false)
{

}

LineDevice::~LineDevice()
{
    if (m_Connected) {
        m_SerialPort->close();
    }
}

bool LineDevice::ConnectDevice(const QString &DeviceName)
{
    m_SerialPort = new AbstractSerial(this);
    if (!m_SerialPort)
        qDebug() << "New AbstractSerial failled";
    connect(m_SerialPort, SIGNAL(readyRead()), this, SLOT(ReadPort()));

    m_SerialPort->setDeviceName(DeviceName);
    if (!m_SerialPort->open(AbstractSerial::ReadWrite)) {
        qDebug() << "Serial device by default: " << m_SerialPort->deviceName() << " open fail: " << m_SerialPort->errorString();
        m_Connected = false;
    } else {
        m_Connected = true;

    }

    QSettings settings;
    settings.beginGroup("LineDevice");
    int configValue = settings.value("BaudRate", AbstractSerial::BaudRate115200).toInt();
    if (!m_SerialPort->setBaudRate((AbstractSerial::BaudRate)configValue)) {
        qDebug() << "Set baud rate " <<  configValue << " error.";
    }
    configValue = settings.value("DataBits", AbstractSerial::DataBits8).toInt();
    if (!m_SerialPort->setDataBits((AbstractSerial::DataBits)configValue)) {
        qDebug() << "Set data bits " <<  configValue << " error.";
    }
    configValue = settings.value("Parity", AbstractSerial::ParityNone).toInt();
    if (!m_SerialPort->setParity((AbstractSerial::Parity)configValue)) {
        qDebug() << "Set parity " <<  configValue << " error.";
    }
    configValue = settings.value("StopBits", AbstractSerial::StopBits1).toInt();
    if (!m_SerialPort->setStopBits((AbstractSerial::StopBits)configValue)) {
        qDebug() << "Set stop bits " <<  configValue << " error.";
    }
    configValue = settings.value("Flow", AbstractSerial::FlowControlOff).toInt();
    if (!m_SerialPort->setFlowControl((AbstractSerial::Flow)configValue)) {
        qDebug() << "Set flow " <<  configValue << " error.";
    }

    return m_Connected;
}

void LineDevice::DisconnectDevice()
{
    if (m_Connected) {
        m_SerialPort->close();
        m_Connected = false;
    }
}

void LineDevice::ReadPort()
{
    QByteArray byteArray = m_SerialPort->readAll();
    emit NewData(byteArray);
}

void LineDevice::WritePort(QByteArray Data)
{
    m_SerialPort->write(Data);
}
