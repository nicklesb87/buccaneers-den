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
#ifndef XBDMWRITER_H
#define XBDMWRITER_H

#include <QXmlStreamWriter>
#include <QModelIndex>

class QModelIndex;
class MacrosModel;
class MacroItem;

class XbdmWriter
{
public:
    XbdmWriter(MacrosModel *macroModel);
    bool WriteFile(QIODevice *device);

private:
    void WriteItem(const MacroItem *macro);
    void WriteFolder(const MacroItem *macro);
    void WriteMacro(const MacroItem *macro);

    QXmlStreamWriter m_XMLWriter;
    MacrosModel *m_MacroModel;
};

#endif // XBDMWRITER_H
