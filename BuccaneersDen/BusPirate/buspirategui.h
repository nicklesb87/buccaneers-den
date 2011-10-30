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
#ifndef BUSPIRATEGUI_H
#define BUSPIRATEGUI_H

#include "../gadgetgui.h"
#include <QWidget>

class QTabBar;
class QStackedLayout;
class BusPirateDevice;
class BusPirateConfig;
class ConsolePage;
class FirmwarePage;


class BusPirateGui : public GadgetGui
{
Q_OBJECT

public:
    explicit BusPirateGui(QWidget *parent = 0);
    virtual ~BusPirateGui();

    void DisplayConfig(const QString &DeviceName);
    bool ConnectDevice(const QString &DeviceName);
    void DisconnectDevice();
    bool IsConnected();
    void FocusInput();

private slots:
    void FunctionChanged(int);
    void UpdateConfig(void);

private:
    QTabBar *m_TabBar;
    QStackedLayout *m_FunctionLayout;
    BusPirateDevice *m_BusPirate;
    BusPirateConfig *m_BPConfigDlg;
    ConsolePage *m_ConsolePage;
    FirmwarePage *m_FirmwarePage;
};

#endif // BUSPIRATEGUI_H
