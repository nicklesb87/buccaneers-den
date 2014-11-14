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
#include <QtSerialPort/QtSerialPort>
#include <QtGui>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QFontDialog>
#include <QColorDialog>

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
    m_PossibleBauds = new QComboBox;
    connect(m_PossibleBauds, SIGNAL(currentIndexChanged(int)), this, SLOT(BaudsChanged(int)));
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
    Q_UNUSED(index);

    bool ok = false;
    int selectedBauds = m_PossibleBauds->currentText().toInt(&ok);
    if (ok) {
        QSettings settings;
        settings.beginGroup("LineDevice");
        settings.setValue("BaudRate", selectedBauds);
    }
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

void LineDeviceConfig::InitBaudsBox(const QString &DeviceName)
{
    if (DeviceName == m_CurrentDeviceName)
        return;

    m_CurrentDeviceName = DeviceName;

    disconnect(m_PossibleBauds, SIGNAL(currentIndexChanged(int)), this, SLOT(BaudsChanged(int)));

    m_PossibleBauds->addItem("4000000");
    m_PossibleBauds->addItem("3500000");
    m_PossibleBauds->addItem("3000000");
    m_PossibleBauds->addItem("2500000");
    m_PossibleBauds->addItem("2000000");
    m_PossibleBauds->addItem("1500000");
    m_PossibleBauds->addItem("1152000");
    m_PossibleBauds->addItem("1000000");
    m_PossibleBauds->addItem("921600");
    m_PossibleBauds->addItem("576000");
    m_PossibleBauds->addItem("500000");
    m_PossibleBauds->addItem("460800");
    m_PossibleBauds->addItem("256000");
    m_PossibleBauds->addItem("230400");
    m_PossibleBauds->addItem("128000");
    m_PossibleBauds->addItem("115200");
    m_PossibleBauds->addItem("57600");
    m_PossibleBauds->addItem("38400");
    m_PossibleBauds->addItem("19200");
    m_PossibleBauds->addItem("9600");
    m_PossibleBauds->addItem("4800");
    m_PossibleBauds->addItem("2400");
    m_PossibleBauds->addItem("1200");
    m_PossibleBauds->addItem("300");

    QSettings settings;
    settings.beginGroup("BusPirate");
    int configValue = settings.value("BaudRate", 115200).toInt();
    int index = m_PossibleBauds->findText(QString::number(configValue));
    if (index != -1)
        m_PossibleBauds->setCurrentIndex(index);

    connect(m_PossibleBauds, SIGNAL(currentIndexChanged(int)), this, SLOT(BaudsChanged(int)));
}

void LineDeviceConfig::InitDataBitsBox(void)
{
    if (m_PossibleDataBits)
        return;

    m_PossibleDataBits = new QComboBox;
    m_PossibleDataBits->insertItem(0, "8", QSerialPort::Data8);
    m_PossibleDataBits->insertItem(1, "7", QSerialPort::Data7);
    m_PossibleDataBits->insertItem(2, "6", QSerialPort::Data6);
    m_PossibleDataBits->insertItem(3, "5", QSerialPort::Data5);

    QSettings settings;
    settings.beginGroup("LineDevice");
    int configValue = settings.value("DataBits", QSerialPort::Data8).toInt();
    switch(configValue) {
    case QSerialPort::Data8:
        m_PossibleDataBits->setCurrentIndex(0);
        break;
    case QSerialPort::Data7:
        m_PossibleDataBits->setCurrentIndex(1);
        break;
    case QSerialPort::Data6:
        m_PossibleDataBits->setCurrentIndex(2);
        break;
    case QSerialPort::Data5:
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
    m_PossibleParity->insertItem(0, "None", QSerialPort::NoParity);
    m_PossibleParity->insertItem(1, "Odd", QSerialPort::OddParity);
    m_PossibleParity->insertItem(2, "Even", QSerialPort::EvenParity);
    m_PossibleParity->insertItem(3, "Mark", QSerialPort::MarkParity);
    m_PossibleParity->insertItem(4, "Space", QSerialPort::SpaceParity);

    QSettings settings;
    settings.beginGroup("LineDevice");
    int configValue = settings.value("Parity", QSerialPort::NoParity).toInt();
    switch(configValue) {
    case QSerialPort::NoParity:
        m_PossibleParity->setCurrentIndex(0);
        break;
    case QSerialPort::OddParity:
        m_PossibleParity->setCurrentIndex(1);
        break;
    case QSerialPort::EvenParity:
        m_PossibleParity->setCurrentIndex(2);
        break;
    case QSerialPort::MarkParity:
        m_PossibleParity->setCurrentIndex(3);
        break;
    case QSerialPort::SpaceParity:
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
    m_PossibleStopBits->insertItem(0, "One", QSerialPort::OneStop);
    m_PossibleStopBits->insertItem(1, "Half", QSerialPort::OneAndHalfStop);
    m_PossibleStopBits->insertItem(2, "Two", QSerialPort::TwoStop);

    QSettings settings;
    settings.beginGroup("LineDevice");
    int configValue = settings.value("StopBits", QSerialPort::OneStop).toInt();
    switch(configValue) {
    case QSerialPort::OneStop:
        m_PossibleStopBits->setCurrentIndex(0);
        break;
    case QSerialPort::OneAndHalfStop:
        m_PossibleStopBits->setCurrentIndex(1);
        break;
    case QSerialPort::TwoStop:
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
    m_PossibleFlow->insertItem(0, "Off", QSerialPort::NoFlowControl);
    m_PossibleFlow->insertItem(1, "Hardware", QSerialPort::HardwareControl);
    m_PossibleFlow->insertItem(2, "FlowControlXonXoff", QSerialPort::SoftwareControl);

    QSettings settings;
    settings.beginGroup("LineDevice");
    int configValue = settings.value("Flow", QSerialPort::NoFlowControl).toInt();
    switch(configValue) {
    case QSerialPort::NoFlowControl:
        m_PossibleStopBits->setCurrentIndex(0);
        break;
    case QSerialPort::HardwareControl:
        m_PossibleStopBits->setCurrentIndex(1);
        break;
    case QSerialPort::SoftwareControl:
        m_PossibleStopBits->setCurrentIndex(2);
        break;
    }
    connect(m_PossibleBauds, SIGNAL(currentIndexChanged(int)), this, SLOT(FlowChanged(int)));
}

void LineDeviceConfig::SetCurrentDevice(const QString &DeviceName)
{
    InitBaudsBox(DeviceName);
}
