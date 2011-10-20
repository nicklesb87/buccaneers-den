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
#include "consolepage.h"
#include "buspiratedevice.h"
#include "buspiratemacroparser.h"
#include "../lib/macrospanel.h"
#include <QtGui>

ConsolePage::ConsolePage(QWidget *parent) :
    QWidget(parent),
    m_InSingleInput(true),
    m_BinaryMode(false)
{
    SetupLog();

    m_ConsoleOutput = new QPlainTextEdit;
    m_ConsoleOutput->setReadOnly(true);
    m_ConsoleOutput->setDocument(m_ConsoleLog);
    m_ConsoleOutput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(m_ConsoleOutput, SIGNAL(textChanged()), this, SLOT(ScrollOutput()));

    m_ToggleInput = new QPushButton(tr("MultiLine"));
    connect(m_ToggleInput, SIGNAL(clicked(void)), this, SLOT(ToggleInput(void)));
    QPushButton *save = new QPushButton(tr("Save Macro"));
    connect(save, SIGNAL(clicked(void)), this, SLOT(SaveInput(void)));
    QPushButton *send = new QPushButton(tr("Send"));
    connect(send, SIGNAL(clicked(void)), this, SLOT(SendInput(void)));
    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_ToggleInput);
    buttonLayout->addWidget(save);
    buttonLayout->addWidget(send);

    m_SingleInput = new QLineEdit;
    m_SingleInput->installEventFilter(this);
    m_SingleInput->setDragEnabled(true);
    connect(m_SingleInput, SIGNAL(returnPressed(void)), this, SLOT(SendInput(void)));

    QVBoxLayout *singleLayout = new QVBoxLayout;
    singleLayout->addStretch();
    singleLayout->addWidget(m_SingleInput);
    m_SinglePage = new QWidget;
    m_SinglePage->setLayout(singleLayout);

    m_MultiInput = new QPlainTextEdit;
    m_MultiInput->installEventFilter(this);
    m_BasicHighlighter = new BASICHighlighter(m_MultiInput->document());

    m_InputLayout = new QStackedLayout;
    m_InputLayout->addWidget(m_SinglePage);
    m_InputLayout->addWidget(m_MultiInput);
    QWidget *inputPanel = new QWidget;
    inputPanel->setLayout(m_InputLayout);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addLayout(buttonLayout);
    bottomLayout->addWidget(inputPanel);
    QWidget *bottomPanel = new QWidget;
    bottomPanel->setLayout(bottomLayout);

    m_IOWidget = new QSplitter(Qt::Vertical);
    m_IOWidget->setChildrenCollapsible(false);
    m_IOWidget->addWidget(m_ConsoleOutput);
    m_IOWidget->setStretchFactor(m_IOWidget->indexOf(m_ConsoleOutput), 100);
    m_IOWidget->addWidget(bottomPanel);

    QString macrosFilename = QDir::homePath();
    macrosFilename += "/.buccaneers/BusPirate/macros.xml";
    m_MacrosWidget = new MacrosPanel(macrosFilename);
    connect(this, SIGNAL(SaveCommand(QString)), m_MacrosWidget, SLOT(NewMacro(QString)));
    connect(m_MacrosWidget, SIGNAL(ExecuteMacro(QString)), this, SLOT(ExecuteMacro(QString)));

    m_MainWidget = new QSplitter(Qt::Horizontal);
    m_MainWidget->addWidget(m_IOWidget);
    m_MainWidget->addWidget(m_MacrosWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(m_MainWidget);

    UpdateTheme();

    setLayout(mainLayout);
}

ConsolePage::~ConsolePage()
{
    QSettings settings;
    settings.beginGroup("BusPirate");
    settings.setValue("IOWidgetGeometry", m_IOWidget->saveGeometry());
    settings.setValue("IOWidgetState", m_IOWidget->saveState());
    settings.setValue("MainWidgetGeometry", m_MainWidget->saveGeometry());
    settings.setValue("MainWidgetState", m_MainWidget->saveState());

    QTextDocumentWriter writer(m_LogFileName);
    writer.setFormat("plaintext");
    if (!writer.write(m_ConsoleLog)) {
        qDebug() << "Unable to write to " << m_LogFileName;
    }
}

void ConsolePage::focusInput()
{
    m_SingleInput->setFocus();
}

void ConsolePage::SetupLog()
{
    QDir home = QDir::home();
    home.cd(".buccaneers/BusPirate/");
    if (!home.exists("Logs")) {
        if (!home.mkdir("Logs"))
            qDebug() << "Unable to create Logs directory" << "\n";
    }
    QDateTime now = QDateTime::currentDateTime();
    m_LogFileName = QDir::homePath();
    m_LogFileName += "/.buccaneers/BusPirate/Logs/";
    m_LogFileName += "BDen_Console-";
#if defined(Q_WS_WIN)
    m_LogFileName += now.toString(Qt::ISODate).replace(":", "-");
#else
    m_LogFileName += now.toString(Qt::ISODate);
#endif
    m_LogFileName += ".log";

    m_ConsoleLog = new QTextDocument;
    QPlainTextDocumentLayout *plainLayout = new QPlainTextDocumentLayout(m_ConsoleLog);
    m_ConsoleLog->setDocumentLayout(plainLayout);
}

void ConsolePage::ScrollOutput()
{
    QScrollBar *scrollBar = m_ConsoleOutput->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

void ConsolePage::SendInput()
{
    QString command;
    if (m_InSingleInput) {
        command = m_SingleInput->text();
        m_LineCommandHistory.AddCommand(command);
        m_SingleInput->clear();
        m_SingleInput->setFocus();
    } else {
        command = m_MultiInput->toPlainText();
        m_MultiLineCommandHistory.AddCommand(command);
        m_MultiInput->clear();
        m_MultiInput->setFocus();
    }
    ExecuteMacro(command);
}

void ConsolePage::SaveInput()
{
    QString command;
    if (m_InSingleInput) {
        command = m_SingleInput->text();
        m_LineCommandHistory.AddCommand(command);
        m_SingleInput->clear();
        m_SingleInput->setFocus();
    } else {
        command = m_MultiInput->toPlainText();
        m_MultiLineCommandHistory.AddCommand(command);
        m_MultiInput->clear();
        m_MultiInput->setFocus();
    }

    emit SaveCommand(command);
}

void ConsolePage::ToggleInput()
{
    if (m_InSingleInput) {
        m_ToggleInput->setText(tr("SingleLine"));
        m_InputLayout->setCurrentIndex(m_InputLayout->indexOf(m_MultiInput));
        m_MultiInput->setFocus();
        m_InSingleInput = false;
    } else {
        m_ToggleInput->setText(tr("MultiLine"));
        m_InputLayout->setCurrentIndex(m_InputLayout->indexOf(m_SinglePage));
        m_SingleInput->setFocus();
        m_InSingleInput = true;
    }
}

void ConsolePage::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event);

    if (m_InSingleInput)
        m_SingleInput->setFocus();
    else
        m_MultiInput->setFocus();
}

bool ConsolePage::eventFilter(QObject *target, QEvent *event)
{
    if (target == m_SingleInput) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Up) {
                m_SingleInput->setText(m_LineCommandHistory.GetPrevious(m_SingleInput->text()));
            } else if (keyEvent->key() == Qt::Key_Down) {
                m_SingleInput->setText(m_LineCommandHistory.GetNext(m_SingleInput->text()));
            }
        }
    }
    if (target == m_MultiInput) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if ((keyEvent->key() == Qt::Key_Up) && QApplication::keyboardModifiers() == Qt::ControlModifier) {
                m_MultiInput->clear();
                m_MultiInput->appendPlainText(m_MultiLineCommandHistory.GetPrevious(m_MultiInput->toPlainText()));
            } else if ((keyEvent->key() == Qt::Key_Down) && QApplication::keyboardModifiers() == Qt::ControlModifier) {
                m_MultiInput->clear();
                m_MultiInput->appendPlainText(m_MultiLineCommandHistory.GetNext(m_MultiInput->toPlainText()));
            }
        }
    }
    return QWidget::eventFilter(target, event);
}

void ConsolePage::ReceiveData(QByteArray Data)
{
    QString newText;
    if (m_BinaryMode) {
        m_LastReceivedData.append(Data);
        QRegExp busPirateRx("Bus Pirate");
        if (busPirateRx.indexIn(m_LastReceivedData) != -1) {
            if (Data.at(0) == 1) {
                newText = PrepareBinaryOutputForDisplay(Data.mid(0, 1));
                newText += PrepareTextOutputForDisplay(Data.mid(1));
            } else {
                newText = PrepareTextOutputForDisplay(Data);
            }

            m_BinaryMode = false;
        } else {
            newText = PrepareBinaryOutputForDisplay(Data);
        }
        if (m_LastReceivedData.length() > 10)
            m_LastReceivedData = m_LastReceivedData.mid(m_LastReceivedData.length()-11);
    } else {
        newText = PrepareTextOutputForDisplay(Data);
    }
    AddToConsoleOutput(newText);
}

/* The Bus Pirate send \r\n for end of line.  QT convert lone \r to \n.
   If the transmission separete both, then we end up with spurious new line. */
QString ConsolePage::PrepareTextOutputForDisplay(const QByteArray &data)
{
    QString text(data);
    if (data.at(data.length()-1) == '\r')
        text.chop(1);
    return text;
}

QString ConsolePage::PrepareBinaryOutputForDisplay(const QByteArray &data)
{
    QString text;
    for (int i = 0; i < data.count(); i++) {
        if (!QChar(data.at(i)).isPrint()) {
            QString number = QString::number(data.at(i), 16);
            number.prepend("{0x");
            number.append("}");
            text.push_back(number);
        } else {
            text.push_back(data.at(i));
        }
    }
    return text;
}

void ConsolePage::UpdateTheme()
{
    LoadFont();
    LoadColor();
    LoadGeometry();
}

void ConsolePage::LoadFont()
{
    QSettings settings;
    settings.beginGroup("BusPirate");
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
    //m_MacrosWidget->setFont(consoleFont);
    m_ConsoleOutput->setFont(consoleFont);
    m_SingleInput->setFont(consoleFont);
    m_MultiInput->setFont(consoleFont);

}

void ConsolePage::LoadColor()
{
    QSettings settings;
    settings.beginGroup("BusPirate");
    QPalette palette;
    if (settings.contains("IO Color")) {
        palette = settings.value("IO Color").value<QPalette>();
    } else {
        palette.setColor(QPalette::Active, QPalette::Base, Qt::black);
        palette.setColor(QPalette::Inactive, QPalette::Base, Qt::black);
        palette.setColor(QPalette::Active, QPalette::Text, Qt::white);
        palette.setColor(QPalette::Inactive, QPalette::Text, Qt::white);
    }
//    m_MacrosWidget->setPalette(palette);
    m_ConsoleOutput->setPalette(palette);
    m_SingleInput->setPalette(palette);
    m_MultiInput->setPalette(palette);
}

void ConsolePage::LoadGeometry()
{
    QSettings settings;
    settings.beginGroup("BusPirate");
    if (settings.contains("IOWidgetGeometry")) {
        m_IOWidget->restoreGeometry(settings.value("IOWidgetGeometry").toByteArray());
        m_IOWidget->restoreState(settings.value("IOWidgetState").toByteArray());
        m_MainWidget->restoreGeometry(settings.value("MainWidgetGeometry").toByteArray());
        m_MainWidget->restoreState(settings.value("MainWidgetState").toByteArray());
    }
}

void ConsolePage::ExecuteMacro(const QString &macro)
{
    QByteArray toSend;
    bool commandIsBinary = BusPirateMacroParser::Parse(macro, toSend);
    if (commandIsBinary || m_BinaryMode) {
        AddToConsoleOutput(PrepareBinaryInputForDisplay(toSend));
        m_BinaryMode = true;
    }
    emit NewCommand(toSend);
}

QString ConsolePage::PrepareBinaryInputForDisplay(const QByteArray &data)
{
    QString text;
    for (int i = 0; i < data.length(); i++) {
        QString number = QString::number(data.at(i), 16);
        number.prepend("[0x");
        number.append("]");
        text.push_back(number);
    }
    return text;
}

void ConsolePage::AddToConsoleOutput(const QString &Text)
{
    QTextCursor cursor = QTextCursor(m_ConsoleLog);
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(Text);

    QTextDocumentWriter writer(m_LogFileName);
    writer.setFormat("plaintext");
    if (!writer.write(m_ConsoleLog)) {
        qDebug() << "Unable to write to " << m_LogFileName;
    }

}
