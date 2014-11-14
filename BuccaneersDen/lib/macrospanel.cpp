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
#include "macrospanel.h"
#include "../lib/macrosview.h"
#include "../lib/macrosmodel.h"
#include "../lib/xbdmreader.h"
#include "../lib/xbdmwriter.h"

#include <QtGui>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>

MacrosPanel::MacrosPanel(QString macrosFileName, QWidget *parent) :
    QWidget(parent)
{
    QLabel *macroLabel = new QLabel(tr("Macros"));
    QStringList labels;
    labels << tr("Name") << tr("Macro");
    m_MacrosWidget = new MacrosView(this);
    connect(m_MacrosWidget, SIGNAL(macroActivated(QString)), this, SLOT(macroActivated(QString)));
    MacrosModel *macroModel = new MacrosModel(this);
    m_MacrosWidget->setModel(macroModel);
    connect(macroModel, SIGNAL(NeedSaving()), this, SLOT(SaveModel()));
    connect(macroModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), m_MacrosWidget, SLOT(dataChanged(const QModelIndex &, const QModelIndex &)));

    QPushButton *loadButton = new QPushButton(tr("Load"));
    loadButton->setShortcut(QKeySequence::Open);
    connect(loadButton, SIGNAL(clicked(void)), this, SLOT(LoadMacrosFrom(void)));

    QPushButton *saveButton = new QPushButton(tr("Save to"));
    saveButton->setShortcut(QKeySequence::SaveAs);
    connect(saveButton, SIGNAL(clicked(void)), this, SLOT(SaveMacrosTo(void)));

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(loadButton);
    buttonsLayout->addWidget(saveButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(macroLabel);
    mainLayout->addWidget(m_MacrosWidget);
    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    m_MacrosFileName = macrosFileName;
    if (QFile::exists(m_MacrosFileName))
        LoadMacros(m_MacrosFileName);
}

void MacrosPanel::setPalette(const QPalette &palette)
{
    MacrosModel *model = reinterpret_cast<MacrosModel*>(m_MacrosWidget->model());
    model->setPalette(palette);
    QWidget::setPalette(palette);
}

void MacrosPanel::LoadMacros(const QString &macrosFileName)
{
    QFile file(macrosFileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("QXmlStream Macros"), tr("Cannot read file %1:\n%2.")
                             .arg(macrosFileName).arg(file.errorString()));
        return;
    }
    MacrosModel *model = reinterpret_cast<MacrosModel*>(m_MacrosWidget->model());
    model->disableSaving();
    XbdmReader reader(model);
    if (!reader.Read(&file)) {
        QMessageBox::warning(this, tr("QXmlStream Macros"), tr("Parse error in file %1:\n\n%2")
                             .arg(macrosFileName).arg(reader.ErrorString()));
    }
    model->enableSaving();
    m_MacrosWidget->resizeColumnToContents(0);
    m_MacrosWidget->resizeColumnToContents(1);
}

void MacrosPanel::SaveMacros(const QString &macrosFileName)
{
    QFile file(macrosFileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("QXmlStream Macros"), tr("Cannot write file %1:\n%2.")
                             .arg(macrosFileName).arg(file.errorString()));
        return;
    }

    XbdmWriter writer;
    writer.WriteFile(&file,reinterpret_cast<MacrosModel *>(m_MacrosWidget->model()));
}

void MacrosPanel::LoadMacrosFrom()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Macros File"),
                                                    QDir::currentPath(),
                                                    tr("XBDM Files (*.xbdm *.xml)"));
    if (fileName.isEmpty())
        return;

    LoadMacros(fileName);
}

void MacrosPanel::SaveMacrosTo()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Macros File"),
                                                    QDir::currentPath(),
                                                    tr("XBDM Files (*.xbdm *.xml)"));
    if (fileName.isEmpty())
        return;

    SaveMacros(fileName);
}

void MacrosPanel::NewMacro(const QString &newMacro)
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("New Macro Name"), QLineEdit::Normal,
                                         tr("Macro Name"), &ok);
    if (ok && !text.isEmpty()) {
        MacrosModel *model = reinterpret_cast<MacrosModel *>(m_MacrosWidget->model());
        QStringList data;
        data << text << newMacro << "macro";
        QModelIndex index = m_MacrosWidget->selectionModel()->currentIndex();
        model->addMacro(data, index.parent());
    }
}

void MacrosPanel::SaveModel()
{
    SaveMacros(m_MacrosFileName);
}

void MacrosPanel::macroActivated(const QString &macro)
{
    emit ExecuteMacro(macro);
}
