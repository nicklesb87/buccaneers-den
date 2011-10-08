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
#ifndef FIRMWAREPAGE_H
#define FIRMWAREPAGE_H

#include <QWidget>

class QLabel;
class QProgressBar;
class BusPirateDevice;

class FirmwarePage : public QWidget
{
Q_OBJECT
public:
    explicit FirmwarePage(BusPirateDevice *BusPirate, QWidget *parent = 0);
    void showEvent(QShowEvent *event);

signals:

public slots:

private slots:
    void UpdateFirmware(void);

private:
    BusPirateDevice *m_BusPirate;
    QLabel *m_BPHardwareVersion;
    QLabel *m_FirmwareVersion;
    QLabel *m_FirmwareRevision;
    QLabel *m_BootloaderVersion;
    QAction *m_UpgradeFirmwareAction;
    QProgressBar *m_UpgradeProgress;
};

#endif // FIRMWAREPAGE_H
