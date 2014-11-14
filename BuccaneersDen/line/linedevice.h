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
#ifndef LINEDEVICE_H
#define LINEDEVICE_H

#include <QWidget>

class QSerialPort;
class QPlainTextEdit;
class QTextDocument;
class QLineEdit;
class QFocusEvent;

class LineDevice : public QWidget
{
Q_OBJECT
public:
    explicit LineDevice(QWidget *parent = 0);
    virtual ~LineDevice();
    bool ConnectDevice(const QString &DeviceName);
    void DisconnectDevice();
    bool IsConnected();

signals:
    void NewData(QByteArray Data);

public slots:
    void WritePort(QByteArray Command);

private slots:
    void ReadPort(void);

private:
    QSerialPort *m_SerialPort;
    bool m_Connected;
};

inline bool LineDevice::IsConnected()
{
    return m_Connected;
}

#endif // LINEDEVICE_H
