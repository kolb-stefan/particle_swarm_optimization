/**
Copyright (C) 2008-2013 Stefan Kolb.

This file is part of the program pso (particle swarm optimization).

The program pso is free software: you can redistribute it
and/or modify it under the terms of the GNU General Public
License as published by the Free Software Foundation, either
version 2 of the License, or (at your option) any later version.

The program pso is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with pso. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FUNCTIONMANAGERDIALOG_H_
#define FUNCTIONMANAGERDIALOG_H_

#include <Qt>
#include <QtGui>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "functionviewer.h"

/**
    Simple dialog to manage saved functions (expressions).
*/
class FunctionManagerDialog : public QDialog
{
        Q_OBJECT
    public:
        FunctionManagerDialog( QWidget *parent, Qt::WindowFlags f = 0 );
        virtual ~FunctionManagerDialog();

        void setExpression( const QString &expr );
        const QString &getExpression();

    public slots:
        void addExpression();
        void deleteExpression();

        void listWidgetClicked( const QModelIndex &index );
        void listWidgetItemSelectionChanged();

        void load();

        void updatePreview( const QString &expr );

    protected:
        void loadFromFile();
        void writeToFile();

        QDialogButtonBox        *ui_buttonbox;
        QListWidget             *ui_listwidget;
        QLineEdit               *ui_expression;
        QPushButton             *ui_add_listing;
        FunctionViewer          *ui_function_viewer;
        QString                 expression;

        std::string             database_filepath;

        struct db_item {std::string expression;};
        std::vector<db_item>    database;
};

#endif
