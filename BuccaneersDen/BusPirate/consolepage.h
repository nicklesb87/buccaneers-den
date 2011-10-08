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
#ifndef CONSOLEPAGE_H
#define CONSOLEPAGE_H

#include "basichighlighter.h"
#include "../lib/commandhistory.h"
#include <QWidget>

class QFocusEvent;
class QLineEdit;
class QPlainTextEdit;
class QPushButton;
class QStackedLayout;
class QTextDocument;
class QSplitter;
class BusPirateDevice;
class MacrosPanel;

class ConsolePage : public QWidget
{
Q_OBJECT
public:
    explicit ConsolePage(QWidget *parent = 0);
    virtual ~ConsolePage();
    void focusInEvent(QFocusEvent *event);
    bool eventFilter(QObject *target, QEvent *event);
    void focusInput();

signals:
    void NewCommand(QByteArray Command);
    void SaveCommand(QString Command);

public slots:
    void ReceiveData(QByteArray Data);
    void UpdateTheme(void);

private slots:
    void ExecuteMacro(const QString &macro);

private:
    void SetupLog();
    void AddToConsoleOutput(const QString &Text);
    QString PrepareTextOutputForDisplay(const QByteArray &data);
    QString PrepareBinaryOutputForDisplay(const QByteArray &data);
    QString PrepareBinaryInputForDisplay(const QByteArray &data);
    void LoadFont();
    void LoadColor();
    void LoadGeometry();

    QPlainTextEdit *m_ConsoleOutput;
    QWidget *m_SinglePage;
    QLineEdit *m_SingleInput;
    QPlainTextEdit *m_MultiInput;
    QStackedLayout *m_InputLayout;
    QPushButton *m_ToggleInput;
    bool m_InSingleInput;
    CommandHistory m_LineCommandHistory;
    CommandHistory m_MultiLineCommandHistory;
    QTextDocument *m_ConsoleLog;
    QString m_LogFileName;
    BASICHighlighter *m_BasicHighlighter;
    MacrosPanel *m_MacrosWidget;
    bool m_BinaryMode;
    QSplitter *m_IOWidget;
    QSplitter *m_MainWidget;
    QByteArray m_LastReceivedData;

private slots:
    void SendInput(void);
    void SaveInput(void);
    void ToggleInput(void);
    void ScrollOutput(void);
};

#endif // CONSOLEPAGE_H
