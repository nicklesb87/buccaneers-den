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
#include <abstractserial.h>
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
    InitBaudsBox();
    speedLayout->addWidget(m_PossibleBauds);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(textLayout);
    mainLayout->addLayout(speedLayout);

    setLayout(mainLayout);
}

void BusPirateConfig::InitBaudsBox()
{
    if (m_PossibleBauds)
        return;

    m_PossibleBauds = new QComboBox;
    m_PossibleBauds->insertItem(0, "921600", AbstractSerial::BaudRate921600);
    m_PossibleBauds->insertItem(1, "460800", AbstractSerial::BaudRate460800);
    m_PossibleBauds->insertItem(2, "230400", AbstractSerial::BaudRate230400);
    m_PossibleBauds->insertItem(3, "115200", AbstractSerial::BaudRate115200);
    m_PossibleBauds->insertItem(4, "57600", AbstractSerial::BaudRate57600);
    m_PossibleBauds->insertItem(5, "38400", AbstractSerial::BaudRate38400);
    m_PossibleBauds->insertItem(6, "19200", AbstractSerial::BaudRate19200);
    m_PossibleBauds->insertItem(7, "9600", AbstractSerial::BaudRate9600);
    m_PossibleBauds->insertItem(8, "4800", AbstractSerial::BaudRate4800);
    m_PossibleBauds->insertItem(9, "2400", AbstractSerial::BaudRate2400);
    m_PossibleBauds->insertItem(10, "1200", AbstractSerial::BaudRate1200);
    m_PossibleBauds->insertItem(11, "300", AbstractSerial::BaudRate300);
    m_PossibleBauds->setCurrentIndex(3);

    QSettings settings;
    settings.beginGroup("BusPirate");
    int configValue = settings.value("BaudRate", AbstractSerial::BaudRate115200).toInt();
    switch(configValue) {
    case AbstractSerial::BaudRate921600:
        m_PossibleBauds->setCurrentIndex(0);
        break;
    case AbstractSerial::BaudRate460800:
        m_PossibleBauds->setCurrentIndex(1);
        break;
    case AbstractSerial::BaudRate230400:
        m_PossibleBauds->setCurrentIndex(2);
        break;
    case AbstractSerial::BaudRate115200:
        m_PossibleBauds->setCurrentIndex(3);
        break;
    case AbstractSerial::BaudRate57600:
        m_PossibleBauds->setCurrentIndex(4);
        break;
    case AbstractSerial::BaudRate38400:
        m_PossibleBauds->setCurrentIndex(5);
        break;
    case AbstractSerial::BaudRate19200:
        m_PossibleBauds->setCurrentIndex(6);
        break;
    case AbstractSerial::BaudRate9600:
        m_PossibleBauds->setCurrentIndex(7);
        break;
    case AbstractSerial::BaudRate4800:
        m_PossibleBauds->setCurrentIndex(8);
        break;
    case AbstractSerial::BaudRate2400:
        m_PossibleBauds->setCurrentIndex(9);
        break;
    case AbstractSerial::BaudRate1200:
        m_PossibleBauds->setCurrentIndex(10);
        break;
    case AbstractSerial::BaudRate300:
        m_PossibleBauds->setCurrentIndex(11);
        break;
    }
    connect(m_PossibleBauds, SIGNAL(currentIndexChanged(int)), this, SLOT(BaudsChanged(int)));
}

void BusPirateConfig::BaudsChanged(int index)
{
    int selectedBauds = m_PossibleBauds->itemData(index).toInt();
    QSettings settings;
    settings.beginGroup("BusPirate");
    settings.setValue("BaudRate", selectedBauds);
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
