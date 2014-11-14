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
#include "firmwarepage.h"
#include "buspiratedevice.h"
#include <QtGui>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QProgressBar>
#include <QFileDialog>

FirmwarePage::FirmwarePage(BusPirateDevice *BusPirate, QWidget *parent) :
    QWidget(parent),
    m_BusPirate(BusPirate)
{
    QLabel *BPVersion = new QLabel(tr("Bus Pirate Version:"));
    m_BPHardwareVersion = new QLabel;
    QHBoxLayout *BPVersionLayout = new QHBoxLayout;
    BPVersionLayout->addWidget(BPVersion);
    BPVersionLayout->addWidget(m_BPHardwareVersion);
    BPVersionLayout->addStretch();

    QLabel *FirmwareVersion = new QLabel(tr("Frimware Version:"));
    m_FirmwareVersion = new QLabel;
    QHBoxLayout *FirmwareVersionLayout = new QHBoxLayout;
    FirmwareVersionLayout->addWidget(FirmwareVersion);
    FirmwareVersionLayout->addWidget(m_FirmwareVersion);
    FirmwareVersionLayout->addStretch();

    QLabel *FirmwareRevision = new QLabel(tr("Frimware Revision:"));
    m_FirmwareRevision = new QLabel;
    QHBoxLayout *FirmwareRevisionLayout = new QHBoxLayout;
    FirmwareRevisionLayout->addWidget(FirmwareRevision);
    FirmwareRevisionLayout->addWidget(m_FirmwareRevision);
    FirmwareRevisionLayout->addStretch();

    QLabel *BootloaderVersion = new QLabel(tr("Bootloader Version:"));
    m_BootloaderVersion = new QLabel;
    QHBoxLayout *BootloaderVersionLayout = new QHBoxLayout;
    BootloaderVersionLayout->addWidget(BootloaderVersion);
    BootloaderVersionLayout->addWidget(m_BootloaderVersion);
    BootloaderVersionLayout->addStretch();

    QPushButton *UpgradeFirmwareButton = new QPushButton(tr("Upgrade Firmware"));
    connect(UpgradeFirmwareButton, SIGNAL(clicked(void)), this, SLOT(UpdateFirmware(void)));
    m_UpgradeProgress = new QProgressBar;
    connect(m_BusPirate, SIGNAL(FirmwareUpgradeNbStep(int)), m_UpgradeProgress, SLOT(setMaximum(int)));
    connect(m_BusPirate, SIGNAL(FirmwareUpgradeStep(int)), m_UpgradeProgress, SLOT(setValue(int)));
    QHBoxLayout *UpgradeLayout = new QHBoxLayout;
    UpgradeLayout->addWidget(UpgradeFirmwareButton);
    UpgradeLayout->addWidget(m_UpgradeProgress);
    m_UpgradeProgress->hide();
    UpgradeLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(BPVersionLayout);
    mainLayout->addLayout(FirmwareVersionLayout);
    mainLayout->addLayout(FirmwareRevisionLayout);
    mainLayout->addLayout(BootloaderVersionLayout);
    mainLayout->addLayout(UpgradeLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);
}

void FirmwarePage::showEvent(QShowEvent *event)
{
    m_BusPirate->ReadVersion();
    m_BPHardwareVersion->setText(m_BusPirate->GetHardwareVersion());
    m_FirmwareVersion->setText(m_BusPirate->GetFirmwareVersion());
    m_FirmwareRevision->setText(QString("%1").arg(m_BusPirate->GetFirmwareRevision()));
    m_BootloaderVersion->setText(QString("%1").arg(m_BusPirate->GetBootloaderVersion()));

    QWidget::showEvent(event);
}

void FirmwarePage::UpdateFirmware()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Firmware File"),
                                                    QDir::currentPath(),
                                                    tr("HEX Files (*.hex)"));
    if (fileName.isEmpty())
        return;

    m_UpgradeProgress->show();
    m_BusPirate->UpdateFirmware(fileName);
    m_UpgradeProgress->hide();

    m_BusPirate->ReadVersion();
    m_BPHardwareVersion->setText(m_BusPirate->GetHardwareVersion());
    m_FirmwareVersion->setText(m_BusPirate->GetFirmwareVersion());
    m_FirmwareRevision->setText(QString("%1").arg(m_BusPirate->GetFirmwareRevision()));
    m_BootloaderVersion->setText(QString("%1").arg(m_BusPirate->GetBootloaderVersion()));
}
