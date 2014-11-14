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
#ifndef VT100DEVICE_H
#define VT100DEVICE_H

#include <QWidget>

class QSerialPort;
class QPlainTextEdit;

class vt100Device : public QWidget
{
Q_OBJECT
public:
    enum ConsoleType {
        vt102
    };

    explicit vt100Device(const QString &DeviceName, QWidget *parent = 0);
    virtual ~vt100Device();

signals:
    void NewData(QByteArray Data);

public slots:
    void WritePort(QByteArray Command);

private slots:
    void ReadPort();

private:
    QSerialPort *m_SerialPort;
    QPlainTextEdit *m_ConsoleDisplay;
};

#endif // VT100DEVICE_H
