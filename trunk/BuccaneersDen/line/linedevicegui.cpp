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
#include "linedevicegui.h"
#include "linedevice.h"
#include "linedeviceconfig.h"
#include "../lib/macrospanel.h"

#include <QtWidgets>

LineDeviceGui::LineDeviceGui(QWidget *parent) :
    GadgetGui(parent),
    m_LDConfigDlg(0)
{
    QDir home = QDir::home();
    home.cd(".buccaneers");
    if (!home.exists("LineDevice")) {
        if (!home.mkdir("LineDevice"))
            qDebug() << "Unable to create Line Device directory" << "\n";
    }
    m_LineDevice = new LineDevice(this);
    SetupLog();

    connect(this, SIGNAL(NewCommand(QByteArray)), m_LineDevice, SLOT(WritePort(QByteArray)));
    connect(m_LineDevice, SIGNAL(NewData(QByteArray)), this, SLOT(ReceiveData(QByteArray)));

    m_ConsoleOutput = new QPlainTextEdit;
    m_ConsoleOutput->setReadOnly(true);
    m_ConsoleOutput->setDocument(m_ConsoleLog);
    m_ConsoleOutput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(m_ConsoleOutput, SIGNAL(textChanged()), this, SLOT(ScrollOutput()));

    QPushButton *save = new QPushButton(tr("Save Macro"));
    connect(save, SIGNAL(clicked(void)), this, SLOT(SaveInput(void)));
    QPushButton *send = new QPushButton(tr("Send"));
    connect(send, SIGNAL(clicked(void)), this, SLOT(SendInput(void)));

    m_LineInput = new QLineEdit;
    m_LineInput->installEventFilter(this);
    m_LineInput->setDragEnabled(true);
    connect(m_LineInput, SIGNAL(returnPressed(void)), this, SLOT(SendInput(void)));

    QString macrosFilename = QDir::homePath();
    macrosFilename += "/.buccaneers/LineDevice/macros.xml";
    MacrosPanel *macrosWidget = new MacrosPanel(macrosFilename);
    connect(this, SIGNAL(SaveCommand(QString)), macrosWidget, SLOT(NewMacro(QString)));
    connect(macrosWidget, SIGNAL(ExecuteMacro(QString)), this, SLOT(ExecuteMacro(QString)));

    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(save);
    buttonLayout->addWidget(send);
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addLayout(buttonLayout);
    bottomLayout->addWidget(m_LineInput);
    QVBoxLayout *IOLayout = new QVBoxLayout;
    IOLayout->addWidget(m_ConsoleOutput);
    IOLayout->addLayout(bottomLayout);
    QWidget *IOWidget = new QWidget;
    IOWidget->setLayout(IOLayout);
    QSplitter *mainWidget = new QSplitter(Qt::Horizontal);
    mainWidget->addWidget(IOWidget);
    mainWidget->addWidget(macrosWidget);
    mainWidget->setStretchFactor(mainWidget->indexOf(IOWidget), 100);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(mainWidget);

    UpdateConfig();

    setLayout(mainLayout);
}

void LineDeviceGui::SetupLog(void)
{
    QDir home = QDir::home();
    home.cd(".buccaneers/LineDevice/");
    if (!home.exists("Logs")) {
        if (!home.mkdir("Logs"))
            qDebug() << "Unable to create Logs directory" << "\n";
    }
    QDateTime now = QDateTime::currentDateTime();
    m_LogFileName = QDir::homePath();
    m_LogFileName += "/.buccaneers/LineDevice/Logs/";
    m_LogFileName += "LD_Console-";
#if defined(Q_OS_WIN)
    m_LogFileName += now.toString(Qt::ISODate).replace(":", "-");
#else
    m_LogFileName += now.toString(Qt::ISODate);
#endif
    m_LogFileName += ".log";

    m_ConsoleLog = new QTextDocument;
    QPlainTextDocumentLayout *plainLayout = new QPlainTextDocumentLayout(m_ConsoleLog);
    m_ConsoleLog->setDocumentLayout(plainLayout);
}

void LineDeviceGui::DisplayConfig(const QString &DeviceName)
{
    if (!m_LDConfigDlg) {
        m_LDConfigDlg = new LineDeviceConfig(this);
        connect(m_LDConfigDlg, SIGNAL(ConfigChanged(void)), this, SLOT(UpdateConfig(void)));
    }
    m_LDConfigDlg->SetCurrentDevice(DeviceName);
    m_LDConfigDlg->show();
    m_LDConfigDlg->raise();
    m_LDConfigDlg->activateWindow();
}

bool LineDeviceGui::ConnectDevice(const QString &DeviceName)
{
    if (!m_LineDevice->ConnectDevice(DeviceName)) {
        QMessageBox::critical(this, "Bus Pirate Device", tr("Unable to open serial port"));
        return false;
    }
    return true;
}

void LineDeviceGui::DisconnectDevice()
{
    m_LineDevice->DisconnectDevice();
}

bool LineDeviceGui::IsConnected(void)
{
    return m_LineDevice->IsConnected();
}

void LineDeviceGui::FocusInput()
{
    m_LineInput->setFocus();
}

void LineDeviceGui::UpdateConfig()
{
    LoadFont();
    LoadColor();
}

void LineDeviceGui::LoadFont()
{
    QSettings settings;
    settings.beginGroup("LineDevice");

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
    m_ConsoleOutput->setFont(consoleFont);
    m_LineInput->setFont(consoleFont);
}

void LineDeviceGui::LoadColor()
{
    QSettings settings;
    settings.beginGroup("LineDevice");

    QPalette palette;
    if (settings.contains("IO Color")) {
        palette = settings.value("IO Color").value<QPalette>();
    } else {
        palette.setColor(QPalette::Active, QPalette::Base, Qt::black);
        palette.setColor(QPalette::Inactive, QPalette::Base, Qt::black);
        palette.setColor(QPalette::Active, QPalette::Text, Qt::white);
        palette.setColor(QPalette::Inactive, QPalette::Text, Qt::white);
    }
    m_ConsoleOutput->setPalette(palette);
    m_LineInput->setPalette(palette);
}

void LineDeviceGui::ScrollOutput(void)
{
    QScrollBar *scrollBar = m_ConsoleOutput->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

void LineDeviceGui::SendInput(void)
{
    QString command = m_LineInput->text();
    m_CommandHistory.AddCommand(command);
    m_LineInput->clear();
    m_LineInput->setFocus();
    ExecuteMacro(command);
}

void LineDeviceGui::SaveInput(void)
{
    QString command = m_LineInput->text();
    m_CommandHistory.AddCommand(command);
    m_LineInput->clear();
    m_LineInput->setFocus();

    emit SaveCommand(command);
}

void LineDeviceGui::ExecuteMacro(const QString &macro)
{
    QByteArray toSend = macro.toLatin1();
    toSend.append('\n');
    emit NewCommand(toSend);
}

void LineDeviceGui::ReceiveData(QByteArray Data)
{
    QString newText(Data);
//    qDebug() << newText << "\n";
    QTextCursor cursor = QTextCursor(m_ConsoleLog);
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(newText);

    QTextDocumentWriter writer(m_LogFileName);
    writer.setFormat("plaintext");
    if (!writer.write(m_ConsoleLog)) {
        qDebug() << "Unable to write to " << m_LogFileName;
    }
}

bool LineDeviceGui::eventFilter(QObject *target, QEvent *event)
{
    if (target == m_LineInput) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Up) {
                m_LineInput->setText(m_CommandHistory.GetPrevious(m_LineInput->text()));
            } else if (keyEvent->key() == Qt::Key_Down) {
                m_LineInput->setText(m_CommandHistory.GetNext(m_LineInput->text()));
            }
        }
    }
    return QWidget::eventFilter(target, event);
}
