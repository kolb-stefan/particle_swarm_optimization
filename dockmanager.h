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

#ifndef DOCKMANAGER_H
#define DOCKMANAGER_H

#include <Qt>
#include <QtGui>

#include "dockwidget.h"

/**
    The DockManager class provides a interface to manages DockWidgets.

    Addinionally it creates for each DockWidget a menuentry to \a dock_menu,
    which is passed to the constructor of DockManager, to enable, close and
    place the DockWidget.
*/
class DockManager : public QObject
{
        Q_OBJECT

    public:
        DockManager( QMainWindow *parent, QMenu *menu );
        ~DockManager();

        void addDock( Qt::DockWidgetArea area, DockWidget *dockwidget );
        QPair<bool, DockWidget *> getDockByTitle( const QString &title );

    public slots:
        void updateDockMenu();
        void dockAction( int id );

    private:
        enum DockAction { CLOSE_ALL, CLOSE, SET_VISIBLE, PLACE_LEFT, PLACE_RIGHT, PLACE_TOP, PLACE_BOTTOM };

        QMenu                       *ui_dock_menu;
        QMainWindow                 *ui_mainwindow;

        QMap<QString, QAction *>    actions_container;
        QMap<int, QPair<QDockWidget *, DockAction> > action_mapper;
        QSignalMapper               dock_signalmapper;

        QVector<DockWidget *>       dockwidget_container;
        int                         map_id;
};

#endif
