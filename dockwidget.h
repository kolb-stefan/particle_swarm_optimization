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

#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <Qt>
#include <QtGui>

/**
    The class DockWidget is a slightly extended version of QDockWidget.
*/
class DockWidget : public QDockWidget
{
        Q_OBJECT

    public:
        DockWidget( const QString &title, QWidget *parent = 0, Qt::WindowFlags flags = 0 );
        DockWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
        virtual ~DockWidget();
    public slots:
        bool close();
        void show();
    signals:
        void closed();
    protected:
        void closeEvent( QCloseEvent *event );
};

#endif
