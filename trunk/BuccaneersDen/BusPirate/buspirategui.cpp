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
#include "buspirategui.h"
#include "buspiratedevice.h"
#include "buspirateconfig.h"
#include "consolepage.h"
#include "firmwarepage.h"
#include <QtGui>
#include <QTabBar>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QMessageBox>

BusPirateGui::BusPirateGui(QWidget *parent) :
    GadgetGui(parent),
    m_BusPirate(0),
    m_BPConfigDlg(0)
{
    QDir home = QDir::home();
    home.cd(".buccaneers");
    if (!home.exists("BusPirate")) {
        if (!home.mkdir("BusPirate"))
            qDebug() << "Unable to create Bus Pirate directory" << "\n";
    }

    m_BusPirate = new BusPirateDevice(this);

    m_ConsolePage = new ConsolePage();
    connect(m_ConsolePage, SIGNAL(NewCommand(QByteArray)), m_BusPirate, SLOT(WritePort(QByteArray)));
    connect(m_BusPirate, SIGNAL(NewData(QByteArray)), m_ConsolePage, SLOT(ReceiveData(QByteArray)));

    m_FirmwarePage = new FirmwarePage(m_BusPirate);

    m_TabBar = new QTabBar;
    int index = m_TabBar->addTab(tr("Console"));
    m_TabBar->setTabData(index, qVariantFromValue(static_cast<void *>(m_ConsolePage)));
    index = m_TabBar->addTab(tr("Firmware"));
    m_TabBar->setTabData(index, qVariantFromValue(static_cast<void *>(m_FirmwarePage)));
    connect(m_TabBar, SIGNAL(currentChanged(int)), this, SLOT(FunctionChanged(int)));

    m_FunctionLayout = new QStackedLayout;
    m_FunctionLayout->addWidget(m_ConsolePage);
    m_FunctionLayout->addWidget(m_FirmwarePage);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_TabBar);
    mainLayout->addLayout(m_FunctionLayout);
    setLayout(mainLayout);
}

BusPirateGui::~BusPirateGui()
{
    disconnect(m_BusPirate, SLOT(WritePort(QByteArray)));
    disconnect(m_ConsolePage, SLOT(ReceiveData(QByteArray)));
}

void BusPirateGui::DisplayConfig(const QString &DeviceName)
{
    if (!m_BPConfigDlg) {
        m_BPConfigDlg = new BusPirateConfig(this);
        connect(m_BPConfigDlg, SIGNAL(ConfigChanged(void)), this, SLOT(UpdateConfig(void)));
    }
    m_BPConfigDlg->SetCurrentDevice(DeviceName);
    m_BPConfigDlg->show();
    m_BPConfigDlg->raise();
    m_BPConfigDlg->activateWindow();
}

void BusPirateGui::UpdateConfig()
{
    m_ConsolePage->UpdateTheme();
}

void BusPirateGui::FocusInput()
{
    m_ConsolePage->focusInput();
}

bool BusPirateGui::ConnectDevice(const QString &DeviceName)
{
    if (!m_BusPirate->ConnectDevice(DeviceName)) {
        QMessageBox::critical(this, "Bus Pirate Device", tr("Unable to open serial port"));
        return false;
    }
    return true;
}

void BusPirateGui::DisconnectDevice()
{
    m_BusPirate->DisconnectDevice();
}

bool BusPirateGui::IsConnected()
{
    return m_BusPirate->IsConnected();
}

void BusPirateGui::FunctionChanged(int index)
{
    QWidget *widget = static_cast<QWidget *>(m_TabBar->tabData(index).value<void *>());
    m_FunctionLayout->setCurrentWidget(widget);
}

