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
#include "mainwindow.h"
#include "BusPirate/buspirategui.h"
#include "line/linedevicegui.h"
#include <serialdeviceenumerator.h>
#include <QtGui>
#include <typeinfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_GadgetGui(0)
{
    QCoreApplication::setOrganizationName("Buccaneer and Co.");
    QCoreApplication::setApplicationName("BuccaneersDen");

    QDir home = QDir::home();
    if (!home.exists(".buccaneers")) {
        if (!home.mkdir(".buccaneers"))
            qDebug() << "Unable to create buccaneers directory" << "\n";
    }

    m_DeviceType = new QComboBox;
    m_DeviceType->insertItem(0, "Bus Pirate");
    m_DeviceType->insertItem(1, "Line console");
    //m_DeviceType->insertItem(2, "vt102");

    m_SerialPort = new QComboBox;
    m_SerialPort->setEditable(true);
    m_SerialEnumerator = new SerialDeviceEnumerator(this);
    CreateAvailableSerialPort();
    connect(m_SerialEnumerator, SIGNAL(hasChanged(QStringList)), this, SLOT(NewSerialPort(QStringList)));

    CreateActions();
    CreateToolBar();

    m_StackedWidget = new QStackedWidget;
    setCentralWidget(m_StackedWidget);

    setWindowTitle("Buccaneer's Den");
}

MainWindow::~MainWindow()
{
}

void MainWindow::NewSerialPort(const QStringList &devicesList)
{
    m_SerialEnumerator->setEnabled(false);
    m_SerialPort->clear();
    QStringList::const_iterator constIterator;
    int Total = 0;
    for (constIterator = devicesList.constBegin(); constIterator != devicesList.constEnd(); ++constIterator)
        m_SerialPort->insertItem(Total++, *constIterator);

    QSettings settings;
    if (settings.contains("SerialPort")) {
        QString defaultPort = settings.value("SerialPort").toString();
        int index = m_SerialPort->findText(defaultPort);
        if (index != -1)
            m_SerialPort->setCurrentIndex(index);
    }
}

void MainWindow::CreateActions()
{
    m_ConfigureAction = new QAction(tr("Configure"), this);
    connect(m_ConfigureAction, SIGNAL(triggered(void)), this, SLOT(PopConfigurationDialog(void)));
    m_ConnectAction = new QAction(tr("Connect"), this);
    connect(m_ConnectAction, SIGNAL(triggered(void)), this, SLOT(ConnectDevice(void)));
    m_DisconnectAction = new QAction(tr("Disconnect"), this);
    connect(m_DisconnectAction, SIGNAL(triggered(void)), this, SLOT(DisconnectDevice(void)));
    m_DisconnectAction->setEnabled(false);
    m_DisconnectAction->setVisible(false);
}

void MainWindow::CreateToolBar()
{
    m_ToolBar = addToolBar(tr("Options"));
    m_ToolBar->setObjectName("toolbar");
    m_ToolBar->addWidget(m_DeviceType);
    m_ToolBar->addWidget(m_SerialPort);
    m_ToolBar->addAction(m_ConfigureAction);
    m_ToolBar->addAction(m_ConnectAction);
    m_ToolBar->addAction(m_DisconnectAction);
}

void MainWindow::PopConfigurationDialog()
{
    CreateGadget();
    m_GadgetGui->DisplayConfig();
}

void MainWindow::ConnectDevice()
{
    m_SerialEnumerator->setEnabled(false);
    CreateGadget();

    if (!m_GadgetGui->ConnectDevice(m_SerialPort->currentText()))
        return;

    QSettings settings;
    restoreGeometry(settings.value("MainGeometry").toByteArray());
    restoreState(settings.value("MainState").toByteArray());

    settings.setValue("SerialPort", m_SerialPort->currentText());
    m_StackedWidget->addWidget(m_GadgetGui);
    m_GadgetGui->FocusInput();

    m_DeviceType->setDisabled(true);
    m_SerialPort->setDisabled(true);
    m_ConfigureAction->setEnabled(false);
    m_ConnectAction->setEnabled(false);
    m_ConnectAction->setVisible(false);
    m_DisconnectAction->setEnabled(true);
    m_DisconnectAction->setVisible(true);
}

void MainWindow::DisconnectDevice()
{
    QSettings settings;
    settings.setValue("MainGeometry", saveGeometry());
    settings.setValue("MainState", saveState());

    m_StackedWidget->removeWidget(m_GadgetGui);
    m_GadgetGui->DisconnectDevice();
    delete m_GadgetGui;
    m_GadgetGui = 0;

    setWindowState(windowState() & !Qt::WindowFullScreen);
    resize(minimumSize());

    m_DeviceType->setDisabled(false);
    m_SerialPort->setDisabled(false);
    m_ConfigureAction->setEnabled(true);
    m_ConnectAction->setEnabled(true);
    m_ConnectAction->setVisible(true);
    m_DisconnectAction->setEnabled(false);
    m_DisconnectAction->setVisible(false);

    CreateAvailableSerialPort();
}

void MainWindow::CreateGadget()
{
    if (0 == m_DeviceType->currentText().compare("Bus Pirate")) {
        if (m_GadgetGui && (typeid(*m_GadgetGui) != typeid(BusPirateGui))) {
            delete m_GadgetGui;
            m_GadgetGui = 0;
        }

        if (!m_GadgetGui)
            m_GadgetGui = new BusPirateGui();

    } else if (0 == m_DeviceType->currentText().compare("Line console")) {
        if (m_GadgetGui && (typeid(*m_GadgetGui) != typeid(LineDeviceGui))) {
            delete m_GadgetGui;
            m_GadgetGui = 0;
        }

        if (!m_GadgetGui)
            m_GadgetGui = new LineDeviceGui();
    }
}

void MainWindow::CreateAvailableSerialPort()
{
    m_SerialEnumerator->setEnabled(true);
    m_SerialPort->clear();
    QStringList devicesList = m_SerialEnumerator->devicesAvailable();
    QStringList::const_iterator constIterator;
    int Total = 0;
    for (constIterator = devicesList.constBegin(); constIterator != devicesList.constEnd(); ++constIterator)
        m_SerialPort->insertItem(Total++, *constIterator);

    QSettings settings;
    if (settings.contains("SerialPort")) {
        QString defaultPort = settings.value("SerialPort").toString();
        int index = m_SerialPort->findText(defaultPort);
        if (index != -1)
            m_SerialPort->setCurrentIndex(index);
    }
}
