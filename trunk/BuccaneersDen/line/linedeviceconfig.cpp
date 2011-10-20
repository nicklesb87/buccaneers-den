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
#include "linedeviceconfig.h"
#include <serialport.h>
#include <QtGui>

LineDeviceConfig::LineDeviceConfig(QWidget *parent) :
    QDialog(parent),
    m_PossibleBauds(0),
    m_PossibleDataBits(0),
    m_PossibleParity(0),
    m_PossibleStopBits(0),
    m_PossibleFlow(0)
{
    QSettings settings;
    settings.beginGroup("LineDevice");

    m_TextExample = new QLineEdit;
    m_TextExample->setText(tr("Text Color Example"));
    m_TextExample->setReadOnly(true);

    QFont consoleFont;
    if (settings.contains("IO Font")) {
        consoleFont = settings.value("IO Font").value<QFont>();
    } else {
#if defined(Q_WS_X11)
        consoleFont.setFamily("Monospace");
#else
        consoleFont.setFamily("Courier New");
#endif
        consoleFont.setPointSize(8);
    }
    m_TextExample->setFont(consoleFont);

    QPalette palette;
    if (settings.contains("IO Color")) {
        palette = settings.value("IO Color").value<QPalette>();
    } else {
        palette.setColor(QPalette::Active, QPalette::Base, Qt::black);
        palette.setColor(QPalette::Inactive, QPalette::Base, Qt::black);
        palette.setColor(QPalette::Active, QPalette::Text, Qt::white);
        palette.setColor(QPalette::Inactive, QPalette::Text, Qt::white);
    }
    m_TextExample->setPalette(palette);

    m_ChangeFont = new QPushButton(tr("Change Font"));
    connect(m_ChangeFont, SIGNAL(clicked(void)), this, SLOT(ChooseFont(void)));
    m_ChangeForeground = new QPushButton(tr("Text Color"));
    connect(m_ChangeForeground, SIGNAL(clicked(void)), this, SLOT(ChooseTextColor(void)));
    m_ChangeBackground = new QPushButton(tr("Background Color"));
    connect(m_ChangeBackground, SIGNAL(clicked(void)), this, SLOT(ChooseBackgroundColor(void)));

    QHBoxLayout *textLayout = new QHBoxLayout;
    textLayout->addWidget(m_TextExample);
    textLayout->addWidget(m_ChangeFont);
    textLayout->addWidget(m_ChangeForeground);
    textLayout->addWidget(m_ChangeBackground);

    QLabel *baudLabel = new QLabel(tr("Baud Rate"));
    InitBaudsBox();
    QHBoxLayout *speedLayout = new QHBoxLayout;
    speedLayout->addWidget(baudLabel);
    speedLayout->addWidget(m_PossibleBauds);

    QLabel *dataBitsLabel = new QLabel(tr("Data Bits"));
    InitDataBitsBox();
    QHBoxLayout *dataBitsLayout = new QHBoxLayout;
    dataBitsLayout->addWidget(dataBitsLabel);
    dataBitsLayout->addWidget(m_PossibleDataBits);

    QLabel *parityLabel = new QLabel(tr("Parity"));
    InitParityBox();
    QHBoxLayout *parityLayout = new QHBoxLayout;
    parityLayout->addWidget(parityLabel);
    parityLayout->addWidget(m_PossibleParity);


    QLabel *stopBitsLabel = new QLabel(tr("Stop Bits"));
    InitStopBitsBox();
    QHBoxLayout *stopBitsLayout = new QHBoxLayout;
    stopBitsLayout->addWidget(stopBitsLabel);
    stopBitsLayout->addWidget(m_PossibleStopBits);

    QLabel *flowLabel = new QLabel(tr("Flow Control"));
    InitFlowBox();
    QHBoxLayout *flowLayout = new QHBoxLayout;
    flowLayout->addWidget(flowLabel);
    flowLayout->addWidget(m_PossibleFlow);


    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(textLayout);
    mainLayout->addLayout(speedLayout);
    mainLayout->addLayout(dataBitsLayout);
    mainLayout->addLayout(parityLayout);
    mainLayout->addLayout(stopBitsLayout);
    mainLayout->addLayout(flowLayout);

    setLayout(mainLayout);
}

void LineDeviceConfig::BaudsChanged(int index)
{
    int selectedBauds = m_PossibleBauds->itemData(index).toInt();
    QSettings settings;
    settings.beginGroup("LineDevice");
    settings.setValue("BaudRate", selectedBauds);
}

void LineDeviceConfig::DataBitsChanged(int index)
{
    int selectedDataBits = m_PossibleDataBits->itemData(index).toInt();
    QSettings settings;
    settings.beginGroup("LineDevice");
    settings.setValue("DataBits", selectedDataBits);
}

void LineDeviceConfig::ParityChanged(int index)
{
    int selectedParity = m_PossibleParity->itemData(index).toInt();
    QSettings settings;
    settings.beginGroup("LineDevice");
    settings.setValue("Parity", selectedParity);
}

void LineDeviceConfig::StopBitsChanged(int index)
{
    int selectedStopBits = m_PossibleStopBits->itemData(index).toInt();
    QSettings settings;
    settings.beginGroup("LineDevice");
    settings.setValue("StopBits", selectedStopBits);
}

void LineDeviceConfig::FlowChanged(int index)
{
    int selectedFlow = m_PossibleFlow->itemData(index).toInt();
    QSettings settings;
    settings.beginGroup("LineDevice");
    settings.setValue("Flow", selectedFlow);
}

void LineDeviceConfig::ChooseFont(void)
{
    QFont font = QFontDialog::getFont(0, m_TextExample->font());
    m_TextExample->setFont(font);
    QSettings settings;
    settings.beginGroup("BusPirate");
    settings.setValue("IO Font", font);
    emit ConfigChanged();
}

void LineDeviceConfig::ChooseTextColor(void)
{
    QPalette palette = m_TextExample->palette();
    QColor color = QColorDialog::getColor(palette.color(QPalette::Active, QPalette::Text), this);
    if (color.isValid()) {
        palette.setColor(QPalette::Active, QPalette::Text, color);
        palette.setColor(QPalette::Inactive, QPalette::Text, color);
        m_TextExample->setPalette(palette);
        QSettings settings;
        settings.beginGroup("BusPirate");
        settings.setValue("IO Color", palette);
        emit ConfigChanged();
    }
}

void LineDeviceConfig::ChooseBackgroundColor(void)
{
    QPalette palette = m_TextExample->palette();
    QColor color = QColorDialog::getColor(palette.color(QPalette::Active, QPalette::Base), this);
    if (color.isValid()) {
        palette.setColor(QPalette::Active, QPalette::Base, color);
        palette.setColor(QPalette::Inactive, QPalette::Base, color);
        m_TextExample->setPalette(palette);
        QSettings settings;
        settings.beginGroup("BusPirate");
        settings.setValue("IO Color", palette);
        emit ConfigChanged();
    }
}

void LineDeviceConfig::InitBaudsBox(void)
{
    if (m_PossibleBauds)
        return;

    m_PossibleBauds = new QComboBox;
#if 0
    m_PossibleBauds->insertItem(0, "4000000", SerialPort::Rate4000000);
    m_PossibleBauds->insertItem(1, "3500000", SerialPort::Rate3500000);
    m_PossibleBauds->insertItem(2, "3000000", SerialPort::Rate3000000);
    m_PossibleBauds->insertItem(3, "2500000", SerialPort::Rate2500000);
    m_PossibleBauds->insertItem(4, "2000000", SerialPort::Rate2000000);
    m_PossibleBauds->insertItem(5, "1500000", SerialPort::Rate1500000);
    m_PossibleBauds->insertItem(6, "1152000", SerialPort::Rate1152000);
    m_PossibleBauds->insertItem(7, "1000000", SerialPort::Rate1000000);
    m_PossibleBauds->insertItem(8, "921600", SerialPort::Rate921600);
    m_PossibleBauds->insertItem(9, "576000", SerialPort::Rate576000);
    m_PossibleBauds->insertItem(10, "500000", SerialPort::Rate500000);
    m_PossibleBauds->insertItem(11, "460800", SerialPort::Rate460800);
    m_PossibleBauds->insertItem(12, "256000", SerialPort::Rate256000);
    m_PossibleBauds->insertItem(13, "230400", SerialPort::Rate230400);
    m_PossibleBauds->insertItem(14, "128000", SerialPort::Rate128000);
#endif
    m_PossibleBauds->insertItem(15, "115200", SerialPort::Rate115200);
    m_PossibleBauds->insertItem(16, "57600", SerialPort::Rate57600);
    m_PossibleBauds->insertItem(17, "38400", SerialPort::Rate38400);
    m_PossibleBauds->insertItem(18, "19200", SerialPort::Rate19200);
    m_PossibleBauds->insertItem(19, "9600", SerialPort::Rate9600);
    m_PossibleBauds->insertItem(20, "4800", SerialPort::Rate4800);
    m_PossibleBauds->insertItem(21, "2400", SerialPort::Rate2400);
    m_PossibleBauds->insertItem(22, "1200", SerialPort::Rate1200);
//    m_PossibleBauds->insertItem(23, "300", SerialPort::BaudRate300);
    m_PossibleBauds->setCurrentIndex(15);

    QSettings settings;
    settings.beginGroup("LineDevice");
    int configValue = settings.value("BaudRate", SerialPort::Rate115200).toInt();
    switch(configValue) {
#if 0
    case SerialPort::Rate4000000:
        m_PossibleBauds->setCurrentIndex(0);
        break;
    case SerialPort::Rate3500000:
        m_PossibleBauds->setCurrentIndex(1);
        break;
    case SerialPort::Rate3000000:
        m_PossibleBauds->setCurrentIndex(2);
        break;
    case SerialPort::Rate2500000:
        m_PossibleBauds->setCurrentIndex(3);
        break;
    case SerialPort::Rate2000000:
        m_PossibleBauds->setCurrentIndex(4);
        break;
    case SerialPort::Rate1500000:
        m_PossibleBauds->setCurrentIndex(5);
        break;
    case SerialPort::Rate1152000:
        m_PossibleBauds->setCurrentIndex(6);
        break;
    case SerialPort::Rate1000000:
        m_PossibleBauds->setCurrentIndex(7);
        break;
    case SerialPort::Rate921600:
        m_PossibleBauds->setCurrentIndex(8);
        break;
    case SerialPort::Rate576000:
        m_PossibleBauds->setCurrentIndex(9);
        break;
    case SerialPort::Rate500000:
        m_PossibleBauds->setCurrentIndex(10);
        break;
    case SerialPort::Rate460800:
        m_PossibleBauds->setCurrentIndex(11);
        break;
    case SerialPort::Rate256000:
        m_PossibleBauds->setCurrentIndex(12);
        break;
    case SerialPort::Rate230400:
        m_PossibleBauds->setCurrentIndex(13);
        break;
    case SerialPort::Rate128000:
        m_PossibleBauds->setCurrentIndex(14);
        break;
#endif
    case SerialPort::Rate115200:
        m_PossibleBauds->setCurrentIndex(15);
        break;
    case SerialPort::Rate57600:
        m_PossibleBauds->setCurrentIndex(16);
        break;
    case SerialPort::Rate38400:
        m_PossibleBauds->setCurrentIndex(17);
        break;
    case SerialPort::Rate19200:
        m_PossibleBauds->setCurrentIndex(18);
        break;
    case SerialPort::Rate9600:
        m_PossibleBauds->setCurrentIndex(19);
        break;
    case SerialPort::Rate4800:
        m_PossibleBauds->setCurrentIndex(20);
        break;
    case SerialPort::Rate2400:
        m_PossibleBauds->setCurrentIndex(21);
        break;
    case SerialPort::Rate1200:
        m_PossibleBauds->setCurrentIndex(22);
        break;
#if 0
    case SerialPort::Rate300:
        m_PossibleBauds->setCurrentIndex(23);
        break;
#endif
    }
    connect(m_PossibleBauds, SIGNAL(currentIndexChanged(int)), this, SLOT(BaudsChanged(int)));
}

void LineDeviceConfig::InitDataBitsBox(void)
{
    if (m_PossibleDataBits)
        return;

    m_PossibleDataBits = new QComboBox;
    m_PossibleDataBits->insertItem(0, "8", SerialPort::Data8);
    m_PossibleDataBits->insertItem(1, "7", SerialPort::Data7);
    m_PossibleDataBits->insertItem(2, "6", SerialPort::Data6);
    m_PossibleDataBits->insertItem(3, "5", SerialPort::Data5);

    QSettings settings;
    settings.beginGroup("LineDevice");
    int configValue = settings.value("DataBits", SerialPort::Data8).toInt();
    switch(configValue) {
    case SerialPort::Data8:
        m_PossibleDataBits->setCurrentIndex(0);
        break;
    case SerialPort::Data7:
        m_PossibleDataBits->setCurrentIndex(1);
        break;
    case SerialPort::Data6:
        m_PossibleDataBits->setCurrentIndex(2);
        break;
    case SerialPort::Data5:
        m_PossibleDataBits->setCurrentIndex(3);
        break;
    }
    connect(m_PossibleBauds, SIGNAL(currentIndexChanged(int)), this, SLOT(DataBitsChanged(int)));
}

void LineDeviceConfig::InitParityBox(void)
{
    if (m_PossibleParity)
        return;

    m_PossibleParity = new QComboBox;
    m_PossibleParity->insertItem(0, "None", SerialPort::NoParity);
    m_PossibleParity->insertItem(1, "Odd", SerialPort::OddParity);
    m_PossibleParity->insertItem(2, "Even", SerialPort::EvenParity);
    m_PossibleParity->insertItem(3, "Mark", SerialPort::MarkParity);
    m_PossibleParity->insertItem(4, "Space", SerialPort::SpaceParity);

    QSettings settings;
    settings.beginGroup("LineDevice");
    int configValue = settings.value("Parity", SerialPort::NoParity).toInt();
    switch(configValue) {
    case SerialPort::NoParity:
        m_PossibleParity->setCurrentIndex(0);
        break;
    case SerialPort::OddParity:
        m_PossibleParity->setCurrentIndex(1);
        break;
    case SerialPort::EvenParity:
        m_PossibleParity->setCurrentIndex(2);
        break;
    case SerialPort::MarkParity:
        m_PossibleParity->setCurrentIndex(3);
        break;
    case SerialPort::SpaceParity:
        m_PossibleParity->setCurrentIndex(4);
        break;
    }
    connect(m_PossibleBauds, SIGNAL(currentIndexChanged(int)), this, SLOT(ParityChanged(int)));
}

void LineDeviceConfig::InitStopBitsBox(void)
{
    if (m_PossibleStopBits)
        return;

    m_PossibleStopBits = new QComboBox;
    m_PossibleStopBits->insertItem(0, "One", SerialPort::OneStop);
    m_PossibleStopBits->insertItem(1, "Half", SerialPort::OneAndHalfStop);
    m_PossibleStopBits->insertItem(2, "Two", SerialPort::TwoStop);

    QSettings settings;
    settings.beginGroup("LineDevice");
    int configValue = settings.value("StopBits", SerialPort::OneStop).toInt();
    switch(configValue) {
    case SerialPort::OneStop:
        m_PossibleStopBits->setCurrentIndex(0);
        break;
    case SerialPort::OneAndHalfStop:
        m_PossibleStopBits->setCurrentIndex(1);
        break;
    case SerialPort::TwoStop:
        m_PossibleStopBits->setCurrentIndex(2);
        break;
    }
    connect(m_PossibleBauds, SIGNAL(currentIndexChanged(int)), this, SLOT(StopBitsChanged(int)));
}

void LineDeviceConfig::InitFlowBox(void)
{
    if (m_PossibleFlow)
        return;

    m_PossibleFlow = new QComboBox;
    m_PossibleFlow->insertItem(0, "Off", SerialPort::NoFlowControl);
    m_PossibleFlow->insertItem(1, "Hardware", SerialPort::HardwareControl);
    m_PossibleFlow->insertItem(2, "FlowControlXonXoff", SerialPort::SoftwareControl);

    QSettings settings;
    settings.beginGroup("LineDevice");
    int configValue = settings.value("Flow", SerialPort::NoFlowControl).toInt();
    switch(configValue) {
    case SerialPort::NoFlowControl:
        m_PossibleStopBits->setCurrentIndex(0);
        break;
    case SerialPort::HardwareControl:
        m_PossibleStopBits->setCurrentIndex(1);
        break;
    case SerialPort::SoftwareControl:
        m_PossibleStopBits->setCurrentIndex(2);
        break;
    }
    connect(m_PossibleBauds, SIGNAL(currentIndexChanged(int)), this, SLOT(FlowChanged(int)));
}
