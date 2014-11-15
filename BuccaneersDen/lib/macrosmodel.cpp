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
#include "macrosmodel.h"

#include <QtWidgets>
#include <QInputDialog>
#include <QStringList>
#include <QMimeData>
#include <QFileIconProvider>

MacrosModel::MacrosModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    QStringList rootData;
    rootData << "Name" << "Code";
    m_RootItem = new MacroItem(rootData);
}

MacrosModel::~MacrosModel()
{
    delete m_RootItem;
}

MacroItem *MacrosModel::nodeFromIndex(const QModelIndex &index) const
{
    if (index.isValid())
        return static_cast<MacroItem*>(index.internalPointer());
    else
        return m_RootItem;
}

QModelIndex MacrosModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    MacroItem *parentItem = nodeFromIndex(parent);
    MacroItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex MacrosModel::addMacro(const QStringList &data, const QModelIndex &parent, int row)
{
    MacroItem *parentItem = nodeFromIndex(parent);
    if (row == -1)
        row = parentItem->childCount();
    beginInsertRows(parent, row, row);
    MacroItem *childItem = new MacroItem(data, parentItem, row);
    endInsertRows();
    if (m_SavingEnabled)
        emit NeedSaving();

    return createIndex(row, 0, childItem);
}

void MacrosModel::deleteMacro(QModelIndex index)
{
    QModelIndex parentIndex = index.parent();
    MacroItem *parentItem = nodeFromIndex(parentIndex);
    beginRemoveRows(parentIndex, index.row(), index.row());
    parentItem->removeChildAt(index.row());
    endRemoveRows();
    if (m_SavingEnabled)
        emit NeedSaving();
}

QModelIndex MacrosModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    MacroItem *childItem = static_cast<MacroItem*>(index.internalPointer());
    MacroItem *parentItem = childItem->parent();

    if (parentItem == m_RootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
 }

int MacrosModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    MacroItem *parentItem = nodeFromIndex(parent);
    return parentItem->childCount();
}

int MacrosModel::columnCount(const QModelIndex &parent) const
{
    MacroItem *macro = nodeFromIndex(parent);
    return macro->columnCount();
}

void MacrosModel::setPalette(const QPalette &palette)
{
    m_Palette = palette;
}

QVariant MacrosModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if ((role != Qt::DisplayRole) &&
        (role != Qt::DecorationRole) &&
        (role != Qt::EditRole) &&
        (role != Qt::BackgroundRole) &&
        (role != Qt::ForegroundRole) &&
        (role != SavingRole))
        return QVariant();

    MacroItem *item = nodeFromIndex(index);
    switch(role) {
    case Qt::BackgroundRole:
        return m_Palette.brush(QPalette::Base);
    case Qt::ForegroundRole:
        return m_Palette.brush(QPalette::Text);
    case Qt::DecorationRole:
        if ((index.column() == NameColumn) && (QString("").compare(item->data(CodeColumn).toString()) == 0)) {
            QFileIconProvider fIcon;
            return fIcon.icon(QFileIconProvider::Folder);
        } else {
            return QVariant();
        }
        break;
    case Qt::DisplayRole:
        if (index.column() == CodeColumn) {
            QString code = item->data(index.column()).toString();
            code.replace(QRegExp("\n.*$"), " ...");
            return code;
        }
        return item->data(index.column());
        break;
    case Qt::EditRole:
        return item->data(index.column());
        break;
    case SavingRole:
        return item->data(index.column());
        break;
    }
    return QVariant();
}

bool MacrosModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.column() != NameColumn)
        return false;

    MacroItem *item = static_cast<MacroItem*>(index.internalPointer());
    if (item) {
        if (role == Qt::EditRole) {
            item->setName(value.toString());
            emit dataChanged(index, index);
            if (m_SavingEnabled)
                emit NeedSaving();
            return true;
        }
    }
    return false;
}

Qt::ItemFlags MacrosModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    if (index.isValid()) {
        MacroItem *item = static_cast<MacroItem*>(index.internalPointer());
        if (item->isFolder())
            defaultFlags |= Qt::ItemIsDropEnabled;

        if (index.column() == NameColumn)
            defaultFlags |= Qt::ItemIsEditable;

        return Qt::ItemIsDragEnabled | defaultFlags;
    } else
        return Qt::ItemIsDropEnabled | defaultFlags;

}

Qt::DropActions MacrosModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList MacrosModel::mimeTypes() const
{
    QStringList types;
    types << "application/vnd.text.list";
    types << "text/plain";
    return types;
}

QMimeData *MacrosModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    foreach (QModelIndex index, indexes) {
        if (index.isValid() && (index.column() == 0)) {
            MacroItem *macro = static_cast<MacroItem*>(index.internalPointer());
            if (indexes.count() == ColumnCount) {
                mimeData->setText(macro->data(CodeColumn).toString());
            }
            stream << macro->data(MacrosModel::NameColumn).toString();
            stream << macro->data(MacrosModel::CodeColumn).toString();
        }
    }
    mimeData->setData("application/vnd.text.list", encodedData);

    return mimeData;
}

bool MacrosModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(column);
    if (action == Qt::IgnoreAction)
        return true;

    if (!(data->hasFormat("application/vnd.text.list") || data->hasFormat("text/plain")))
        return false;

    if (data->hasFormat("application/vnd.text.list")) {
        AddMacroFromVnd(data, row, parent);
    } else if (data->hasFormat("text/plain")) {
        AddMacroFromPlainText(data, row, parent);
    }

    return true;
}

void MacrosModel::AddMacroFromVnd(const QMimeData *data, int row, const QModelIndex &parent)
{
    QByteArray encodedData = data->data("application/vnd.text.list");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QStringList newItems;
    int column = 0;
    while (!stream.atEnd()) {
        QString text;
        stream >> text;
        newItems << text;
        ++column;
        if (column == ColumnCount) {
            addMacro(newItems, parent, row);
            column = 0;
        }
    }
}

void MacrosModel::AddMacroFromPlainText(const QMimeData *data, int row, const QModelIndex &parent)
{
    QString macroCode = data->text();
    QStringList newItem;
    newItem << "New Macro";
    newItem << macroCode;
    addMacro(newItem, parent, row);
}

QVariant MacrosModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_RootItem->data(section);

    return QVariant();
}

bool MacrosModel::removeRows(int row, int count, const QModelIndex &parent)
{
    MacroItem *parentItem = nodeFromIndex(parent);
    beginRemoveRows(parent, row, row+count-1);
    parentItem->removeChildAt(row);
    endRemoveRows();
    if (m_SavingEnabled)
        emit NeedSaving();
    return true;
}

void MacrosModel::disableSaving()
{
    m_SavingEnabled = false;
}

void MacrosModel::enableSaving()
{
    m_SavingEnabled = true;
}
