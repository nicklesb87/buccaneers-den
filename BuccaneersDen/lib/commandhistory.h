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
#ifndef COMMANDHISTORY_H
#define COMMANDHISTORY_H

#include <QList>
#include <QString>

class CommandHistory
{
public:
    CommandHistory();
    virtual ~CommandHistory();

    void AddCommand(const QString &command);
    QString GetPrevious(const QString &current);
    QString GetNext(const QString &current);

private:
    QList<QString> m_LineCommandHistory;
    QList<QString>::const_iterator m_CommandHistoryPosition;
    QString m_CurrentLine;
};

#endif // COMMANDHISTORY_H
