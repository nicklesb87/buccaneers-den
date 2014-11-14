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
#include <QtSerialPort/QtSerialPort>
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
    m_SerialPort = new QSerialPort(DeviceName, this);
    if (!m_SerialPort)
        qDebug() << "New SerialPort failled";
    connect(m_SerialPort, SIGNAL(readyRead()), this, SLOT(ReadPort()));

    if (!m_SerialPort->open(QSerialPort::ReadWrite)) {
        qDebug() << "Serial device by default: " << m_SerialPort->portName() << " open fail: " << m_SerialPort->errorString();
        m_Connected = false;
    } else {
        m_Connected = true;

    }

    QSettings settings;
    settings.beginGroup("LineDevice");
    int configValue = settings.value("BaudRate", QSerialPort::Baud115200).toInt();
    if (!m_SerialPort->setBaudRate((QSerialPort::BaudRate)configValue)) {
        qDebug() << "Set baud rate " <<  configValue << " error.";
    }
    configValue = settings.value("DataBits", QSerialPort::Data8).toInt();
    if (!m_SerialPort->setDataBits((QSerialPort::DataBits)configValue)) {
        qDebug() << "Set data bits " <<  configValue << " error.";
    }
    configValue = settings.value("Parity", QSerialPort::NoParity).toInt();
    if (!m_SerialPort->setParity((QSerialPort::Parity)configValue)) {
        qDebug() << "Set parity " <<  configValue << " error.";
    }
    configValue = settings.value("StopBits", QSerialPort::OneStop).toInt();
    if (!m_SerialPort->setStopBits((QSerialPort::StopBits)configValue)) {
        qDebug() << "Set stop bits " <<  configValue << " error.";
    }
    configValue = settings.value("Flow", QSerialPort::NoFlowControl).toInt();
    if (!m_SerialPort->setFlowControl((QSerialPort::FlowControl)configValue)) {
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
