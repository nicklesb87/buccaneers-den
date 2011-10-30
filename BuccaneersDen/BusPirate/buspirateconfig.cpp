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
#include "buspirateconfig.h"
#include <serialport.h>
#include <serialportinfo.h>
#include <QtGui>

BusPirateConfig::BusPirateConfig(QWidget *parent) :
    QDialog(parent),
    m_PossibleBauds(0)
{
    QSettings settings;
    settings.beginGroup("BusPirate");

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

    QHBoxLayout *speedLayout = new QHBoxLayout;
    QLabel *baudLabel = new QLabel(tr("Baud Rate"));
    speedLayout->addWidget(baudLabel);
    m_PossibleBauds = new QComboBox;
    connect(m_PossibleBauds, SIGNAL(currentIndexChanged(int)), this, SLOT(BaudsChanged(int)));
    speedLayout->addWidget(m_PossibleBauds);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(textLayout);
    mainLayout->addLayout(speedLayout);

    setLayout(mainLayout);
}

void BusPirateConfig::InitBaudsBox(const QString &DeviceName)
{
    if (DeviceName == m_CurrentDeviceName)
        return;

    m_CurrentDeviceName = DeviceName;

    disconnect(m_PossibleBauds, SIGNAL(currentIndexChanged(int)), this, SLOT(BaudsChanged(int)));
    m_PossibleBauds->addItem("921600");
    m_PossibleBauds->addItem("460800");
    m_PossibleBauds->addItem("230400");
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

void BusPirateConfig::BaudsChanged(int index)
{
    Q_UNUSED(index);

    bool ok = false;
    int selectedBauds = m_PossibleBauds->currentText().toInt(&ok);
    if (ok) {
        QSettings settings;
        settings.beginGroup("BusPirate");
        settings.setValue("BaudRate", selectedBauds);
    }
}

void BusPirateConfig::ChooseFont()
{
    QFont font = QFontDialog::getFont(0, m_TextExample->font());
    m_TextExample->setFont(font);
    QSettings settings;
    settings.beginGroup("BusPirate");
    settings.setValue("IO Font", font);
    emit ConfigChanged();
}

void BusPirateConfig::ChooseTextColor()
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

void BusPirateConfig::ChooseBackgroundColor()
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

void BusPirateConfig::SetCurrentDevice(const QString &DeviceName)
{
    InitBaudsBox(DeviceName);
}
