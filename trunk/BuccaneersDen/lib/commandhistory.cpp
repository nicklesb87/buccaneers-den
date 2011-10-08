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
#include "commandhistory.h"

CommandHistory::CommandHistory()
{
    m_CommandHistoryPosition = m_LineCommandHistory.constEnd();
}

CommandHistory::~CommandHistory()
{

}

void CommandHistory::AddCommand(const QString &command)
{
    m_LineCommandHistory.prepend(command);
    m_CommandHistoryPosition = m_LineCommandHistory.constEnd();
}

QString CommandHistory::GetPrevious(const QString &current)
{
    if (!m_LineCommandHistory.empty()) {
        if (m_CommandHistoryPosition == m_LineCommandHistory.constEnd()) {
            m_CurrentLine = current;
            m_CommandHistoryPosition = m_LineCommandHistory.constBegin();
        } else {
            ++m_CommandHistoryPosition;
        }
        if (m_CommandHistoryPosition == m_LineCommandHistory.constEnd())
            return m_CurrentLine;
        else
            return *m_CommandHistoryPosition;
    }
    return QString("");
}

QString CommandHistory::GetNext(const QString &current)
{
    if (!m_LineCommandHistory.empty()) {
        if (m_CommandHistoryPosition == m_LineCommandHistory.constEnd()) {
            m_CurrentLine = current;
        }
        if (m_CommandHistoryPosition == m_LineCommandHistory.constBegin()) {
            m_CommandHistoryPosition = m_LineCommandHistory.constEnd();
            return m_CurrentLine;
        } else {
            --m_CommandHistoryPosition;
            return *m_CommandHistoryPosition;
        }
    }
    return QString("");
}
