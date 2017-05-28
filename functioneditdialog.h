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

#ifndef FUNCTIONDIALOG_H_
#define FUNCTIONDIALOG_H_

#include <Qt>
#include <QtGui>

#include <vector>
#include <string>
#include <fstream>

#include "functionviewer.h"

class FunctionEditDialogWorker : public QThread
{
        Q_OBJECT
    public:
        FunctionEditDialogWorker( QObject *parent = 0 );
        virtual ~FunctionEditDialogWorker( );

        void setExpression( QString expr );
        void run();
    signals:
        void equationChecked( QString expr );

    protected:
        QString expression;
};

/**
    Simple dialog to edit the currently used expression (function).
*/
class FunctionEditDialog : public QDialog
{
        Q_OBJECT
    public:
        FunctionEditDialog( QWidget *parent, Qt::WindowFlags f = 0 );
        virtual ~FunctionEditDialog();
    public slots:
        void setExpression( const std::string &text );
        std::string getExpression( void );

        void updatePreview( const QString &expr );
        void timerTimeOut();

        void expressionChanged();

    protected:
        QTextEdit           *ui_expression_edit;
        QDialogButtonBox    *ui_buttonbox;
        FunctionViewer      *ui_function_viewer;

        QTimer              timer;

        FunctionEditDialogWorker worker;
};

#endif
