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
#ifndef MACROSPANEL_H
#define MACROSPANEL_H

#include <QWidget>
#include <QModelIndex>

class QAction;
class QTreeView;

class MacrosPanel : public QWidget
{
Q_OBJECT
public:
    explicit MacrosPanel(QString macrosFilename, QWidget *parent = 0);
    void setPalette(const QPalette &palette);

signals:
    void ExecuteMacro(QString);

public slots:
    void NewMacro(const QString &newMacro);
    void SaveModel();

private slots:
    void LoadMacrosFrom(void);
    void SaveMacrosTo(void);
    void macroActivated(const QString &macro);

private:
    void LoadMacros(const QString &macrosFileName);
    void SaveMacros(const QString &macrosFileName);

    QTreeView *m_MacrosWidget;
    QString m_MacrosFileName;
};

#endif // MACROSPANEL_H
