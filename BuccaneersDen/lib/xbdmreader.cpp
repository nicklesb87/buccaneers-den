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
#include "xbdmreader.h"
#include "macrosmodel.h"

#include <QtCore>

XbdmReader::XbdmReader(MacrosModel *macroModel)
    : m_MacroModel(macroModel)
{

}

bool XbdmReader::Read(QIODevice *device)
{
    m_XMLReader.setDevice(device);

    if (m_XMLReader.readNextStartElement()) {
        if (m_XMLReader.name() == "xbdm" && m_XMLReader.attributes().value("version") == "1.0")
            ReadXBDM();
        else
            m_XMLReader.raiseError(QObject::tr("The file is not an XBDM version 1.0 file."));
    }

    return !m_XMLReader.error();
}

QString XbdmReader::ErrorString() const
{
    return QObject::tr("%1\nLine %2, column %3")
            .arg(m_XMLReader.errorString())
            .arg(m_XMLReader.lineNumber())
            .arg(m_XMLReader.columnNumber());
}

void XbdmReader::ReadXBDM()
{
    Q_ASSERT(m_XMLReader.isStartElement() && m_XMLReader.name() == "xbdm");

    while (m_XMLReader.readNextStartElement()) {
        if (m_XMLReader.name() == "folder")
            ReadFolder();
        else if (m_XMLReader.name() == "macro")
            ReadMacro();
        else
            m_XMLReader.skipCurrentElement();
    }
}

void XbdmReader::ReadFolder(const QModelIndex &parent)
{
    Q_ASSERT(m_XMLReader.isStartElement() && m_XMLReader.name() == "folder");

    QStringList data;
    data << m_XMLReader.attributes().value("name").toString();
    data << "";
    QModelIndex folder = m_MacroModel->addMacro(data, parent);
    while (m_XMLReader.readNextStartElement()) {
        if (m_XMLReader.name() == "folder")
            ReadFolder(folder);
        else if (m_XMLReader.name() == "macro")
            ReadMacro(folder);
        else
            m_XMLReader.skipCurrentElement();
    }
}

void XbdmReader::ReadMacro(const QModelIndex &parent)
{
    Q_ASSERT(m_XMLReader.isStartElement() && m_XMLReader.name() == "macro");

    QStringList data;
    data << m_XMLReader.attributes().value("name").toString();
    data << m_XMLReader.readElementText();
    m_MacroModel->addMacro(data, parent);
}
