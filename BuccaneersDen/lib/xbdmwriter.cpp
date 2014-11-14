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
#include "xbdmwriter.h"
#include "macrosmodel.h"

#include <QtGui>

XbdmWriter::XbdmWriter()
{
    m_XMLWriter.setAutoFormatting(true);
}

bool XbdmWriter::WriteFile(QIODevice *device, MacrosModel *MacroModel)
{
    m_XMLWriter.setDevice(device);

    m_XMLWriter.writeStartDocument();
    m_XMLWriter.writeDTD("<!DOCTYPE xbdm>");
    m_XMLWriter.writeStartElement("xbdm");
    m_XMLWriter.writeAttribute("version", "1.0");
    for (int i = 0; i < MacroModel->rowCount(); ++i)
    {
        QModelIndex current = MacroModel->index(i, 0);
        MacroItem *item = static_cast<MacroItem*>(current.internalPointer());
        WriteItem(item);
    }
    m_XMLWriter.writeEndDocument();
    return true;
}

void XbdmWriter::WriteItem(const MacroItem *macro)
{
    if (macro->isFolder()) {
        WriteFolder(macro);
    } else if (macro->isMacro()) {
        WriteMacro(macro);
    } else if (macro->isSeparator()) {
        m_XMLWriter.writeEmptyElement("separator");
    }
}

void XbdmWriter::WriteFolder(const MacroItem *macro)
{
    m_XMLWriter.writeStartElement("folder");
    m_XMLWriter.writeAttribute("name", macro->data(MacrosModel::NameColumn).toString());
    for (int i = 0; i < macro->childCount(); ++i)
    {
        WriteItem(macro->child(i));
    }
    m_XMLWriter.writeEndElement();
}

void XbdmWriter::WriteMacro(const MacroItem *macro)
{
    m_XMLWriter.writeStartElement("macro");
    m_XMLWriter.writeAttribute("name", macro->data(MacrosModel::NameColumn).toString());
    m_XMLWriter.writeCharacters(macro->data(MacrosModel::CodeColumn).toString());
    m_XMLWriter.writeEndElement();
}
