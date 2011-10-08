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
#include "macrosview.h"
#include "../lib/macrosmodel.h"

#include <QApplication>
#include <QModelIndex>
#include <QMouseEvent>
#include <QMenu>

MacrosView::MacrosView(QWidget *parent)
    : QTreeView(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragDropMode(QAbstractItemView::DragDrop);
    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    viewport()->setAcceptDrops(true);
    setDropIndicatorShown(true);
    connect(this, SIGNAL(activated(QModelIndex)), this, SLOT(itemActivated(QModelIndex)));
}

void MacrosView::itemActivated(QModelIndex index)
{
    if (index.isValid()) {
        MacroItem *item = static_cast<MacroItem*>(index.internalPointer());
        if (item->isMacro())
            emit macroActivated(item->data(MacrosModel::CodeColumn).toString());
    }
}

void MacrosView::activateMacro()
{
    QModelIndex index = selectionModel()->currentIndex();
    if (index.isValid()) {
        MacroItem *item = static_cast<MacroItem*>(index.internalPointer());
        if (item->isMacro())
            emit macroActivated(item->data(MacrosModel::CodeColumn).toString());
    }
}

void MacrosView::createFolder()
{
    QStringList data;
    data << "New Folder" << "";
    QModelIndex index = selectionModel()->currentIndex();
    MacrosModel *macrosModel = reinterpret_cast<MacrosModel *>(model());
    QModelIndex folderIndex = macrosModel->addMacro(data, index.parent());
    edit(folderIndex);
}

void MacrosView::deleteMacro()
{
    QModelIndex index = selectionModel()->currentIndex();
    if (index.isValid()) {
        MacrosModel *macrosModel = reinterpret_cast<MacrosModel *>(model());
        macrosModel->deleteMacro(index);
    }
}

void MacrosView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = new QMenu(this);
    QModelIndex index = indexAt(event->pos());
    if (index.isValid()) {
        MacroItem *item = static_cast<MacroItem*>(index.internalPointer());
        if (item->isMacro()) {
            QAction *activateMacroAction = menu->addAction(QIcon(), tr("Activate"));
            connect(activateMacroAction, SIGNAL(triggered()), this, SLOT(activateMacro()));
        }
    }
    QAction *createFolderAction = menu->addAction(QIcon(), tr("CreateFolder"));
    connect(createFolderAction, SIGNAL(triggered()), this, SLOT(createFolder()));
    QAction *deleteMacroAction = menu->addAction(QIcon(), tr("Delete"));
    connect(deleteMacroAction, SIGNAL(triggered()), this, SLOT(deleteMacro()));
    menu->addSeparator();

    menu->exec(QCursor::pos());
}
