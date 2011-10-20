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
#include "vt100device.h"
#include <serialport.h>

#include <QtGui>

vt100Device::vt100Device(const QString &DeviceName, QWidget *parent) :
    QWidget(parent)
{
    m_SerialPort = new SerialPort(DeviceName);
    connect(m_SerialPort, SIGNAL(readyRead()), this, SLOT(ReadPort()));

    if (!m_SerialPort->open(SerialPort::ReadWrite)) {
        qDebug() << "Serial device by default: " << m_SerialPort->portName() << " open fail.";
    }

    QSettings settings;
    settings.beginGroup("SerialConfig");
    int configValue = settings.value("BaudRate", SerialPort::Rate115200).toInt();
    if (!m_SerialPort->setRate((SerialPort::Rate)configValue)) {
        qDebug() << "Set baud rate " <<  configValue << " error.";
    }
    configValue = settings.value("DataBits", SerialPort::Data8).toInt();
    if (!m_SerialPort->setDataBits((SerialPort::DataBits)configValue)) {
        qDebug() << "Set data bits " <<  configValue << " error.";
    }
    configValue = settings.value("Parity", SerialPort::NoParity).toInt();
    if (!m_SerialPort->setParity((SerialPort::Parity)configValue)) {
        qDebug() << "Set parity " <<  configValue << " error.";
    }
    configValue = settings.value("StopBits", SerialPort::OneStop).toInt();
    if (!m_SerialPort->setStopBits((SerialPort::StopBits)configValue)) {
        qDebug() << "Set stop bits " <<  configValue << " error.";
    }
    configValue = settings.value("FlowControl", SerialPort::NoFlowControl).toInt();
    if (!m_SerialPort->setFlowControl((SerialPort::FlowControl)configValue)) {
        qDebug() << "Set flow " <<  configValue << " error.";
    }

    QVBoxLayout *mainLayout = new QVBoxLayout;
    m_ConsoleDisplay = new QPlainTextEdit;
    QFont consoleFont;
#if defined(Q_WS_X11)
    consoleFont.setFamily("Monospace");
#else
    consoleFont.setFamily("Courier New");
#endif
    m_ConsoleDisplay->setFont(consoleFont);
    connect(m_ConsoleDisplay, SIGNAL(textChanged()), this, SLOT(ScrollOutput()));

    mainLayout->addWidget(m_ConsoleDisplay);
    parent->setLayout(mainLayout);
}

vt100Device::~vt100Device()
{
    m_SerialPort->close();
    delete m_SerialPort;
}

void vt100Device::ReadPort()
{
    QByteArray byteArray = m_SerialPort->readAll();
//    qDebug() << "Read : " << byteArray.size() << " bytes";
    emit NewData(byteArray);
}

void vt100Device::WritePort(QByteArray Data)
{
//    qDebug() << "Sending command : " << Command;
    m_SerialPort->write(Data);
}
