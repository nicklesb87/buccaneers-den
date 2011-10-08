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
#ifndef LINEDEVICEGUI_H
#define LINEDEVICEGUI_H

#include "../gadgetgui.h"
#include "../lib/commandhistory.h"
#include <QWidget>

class QPlainTextEdit;
class QLineEdit;
class QTextDocument;
class LineDevice;
class LineDeviceConfig;

class LineDeviceGui : public GadgetGui
{
Q_OBJECT
public:
    explicit LineDeviceGui(QWidget *parent = 0);

    void DisplayConfig();
    bool ConnectDevice(const QString &DeviceName);
    void DisconnectDevice();
    bool IsConnected();
    void FocusInput();
    bool eventFilter(QObject *target, QEvent *event);

signals:
    void NewCommand(QByteArray Command);
    void SaveCommand(QString Command);

public slots:
    void ReceiveData(QByteArray Data);

private slots:
    void UpdateConfig(void);
    void SendInput(void);
    void SaveInput(void);
    void ScrollOutput(void);
    void ExecuteMacro(const QString &macro);

private:
    void SetupLog();
    void LoadFont();
    void LoadColor();

    LineDevice *m_LineDevice;
    LineDeviceConfig *m_LDConfigDlg;
    QPlainTextEdit *m_ConsoleOutput;
    QLineEdit *m_LineInput;
    CommandHistory m_CommandHistory;
    QTextDocument *m_ConsoleLog;
    QString m_LogFileName;

};

#endif // LINEDEVICEGUI_H
