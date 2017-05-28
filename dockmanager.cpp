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

#include "dockmanager.h"

DockManager::DockManager( QMainWindow *parent, QMenu *menu )
{
    ui_mainwindow = parent;
    ui_dock_menu = menu;

    updateDockMenu();
}

DockManager::~DockManager()
{

}

/**
    Adds \a dockwidget to the list of widgets and adds a menu entry to the
    provided QMenu. The widget is added to the dockarea \a area of the
    provided QMainWindow.
*/
void DockManager::addDock( Qt::DockWidgetArea area, DockWidget *dockwidget )
{
    connect( dockwidget, SIGNAL( closed() ), this, SLOT( updateDockMenu() ) );
    ui_mainwindow->addDockWidget( area, dockwidget );
    dockwidget_container.push_back( dockwidget );
    updateDockMenu();
}

void DockManager::updateDockMenu()
{
    ui_dock_menu->clear();
    dock_signalmapper.disconnect();
    connect( &dock_signalmapper, SIGNAL( mapped( int ) ), this, SLOT( dockAction( int ) ) );

    for( QMap<int, QPair<QDockWidget *, DockAction> >::iterator it = action_mapper.begin(); it != action_mapper.end(); it++ )
    {
        dock_signalmapper.removeMappings( dock_signalmapper.mapping( it->first ) );
    }

    action_mapper.clear();
    map_id = 0;

    bool enable = ( !dockwidget_container.empty() );

    actions_container["dock::Animation"] = ui_dock_menu->addAction( tr( "Animation" ) );
    actions_container["dock::Animation"]->setCheckable( true );
    actions_container["dock::Animation"]->setChecked( ui_mainwindow->isAnimated() );
    connect( actions_container["dock::Animation"], SIGNAL( triggered( bool ) ), ui_mainwindow, SLOT( setAnimated( bool ) ) );
    ui_dock_menu->addSeparator();

    actions_container["dock::close all"] = ui_dock_menu->addAction( tr( "Close All" ) );
    actions_container["dock::close all"]->setEnabled( enable );
    connect( actions_container["dock::close all"], SIGNAL( triggered() ), &dock_signalmapper, SLOT( map() ) );
    dock_signalmapper.setMapping( actions_container["dock::close all"], map_id );
    action_mapper[map_id] = qMakePair( ( QDockWidget * )0, CLOSE_ALL );
    map_id++;
    ui_dock_menu->addSeparator();

    for( QVector<DockWidget *>::iterator it = dockwidget_container.begin(); it != dockwidget_container.end(); it++ )
    {
        QDockWidget *dockwidget = *it;
        bool visible = !dockwidget->isHidden();

        if( visible )
        {
            QAction *tmp_action = NULL;
            QMenu *menu = ui_dock_menu->addMenu( dockwidget->windowTitle() );
            tmp_action = menu->addAction( "Close" );
            connect( tmp_action, SIGNAL( triggered() ), &dock_signalmapper, SLOT( map() ) );
            dock_signalmapper.setMapping( tmp_action, map_id );
            action_mapper[map_id] = qMakePair( dockwidget, CLOSE );
            map_id++;
            menu->addSeparator();
            tmp_action = menu->addAction( "Place on Left" );
            connect( tmp_action, SIGNAL( triggered() ), &dock_signalmapper, SLOT( map() ) );
            dock_signalmapper.setMapping( tmp_action, map_id );
            action_mapper[map_id] = qMakePair( dockwidget, PLACE_LEFT );
            map_id++;

            tmp_action = menu->addAction( "Place on Right" );
            connect( tmp_action, SIGNAL( triggered() ), &dock_signalmapper, SLOT( map() ) );
            dock_signalmapper.setMapping( tmp_action, map_id );
            action_mapper[map_id] = qMakePair( dockwidget, PLACE_RIGHT );
            map_id++;

            tmp_action = menu->addAction( "Place on Top" );
            connect( tmp_action, SIGNAL( triggered() ), &dock_signalmapper, SLOT( map() ) );
            dock_signalmapper.setMapping( tmp_action, map_id );
            action_mapper[map_id] = qMakePair( dockwidget, PLACE_TOP );
            map_id++;

            tmp_action = menu->addAction( "Place on Bottom" );
            connect( tmp_action, SIGNAL( triggered() ), &dock_signalmapper, SLOT( map() ) );
            dock_signalmapper.setMapping( tmp_action, map_id );
            action_mapper[map_id] = qMakePair( dockwidget, PLACE_BOTTOM );
            map_id++;
        }
        else
        {
            QAction *tmp_action  = ui_dock_menu->addAction( dockwidget->windowTitle() );
            tmp_action->setCheckable( true );
            tmp_action->setChecked( visible );
            tmp_action->setEnabled( dockwidget->isEnabled() );
            connect( tmp_action, SIGNAL( triggered() ), &dock_signalmapper, SLOT( map() ) );
            dock_signalmapper.setMapping( tmp_action, map_id );
            action_mapper[map_id] = qMakePair( dockwidget, SET_VISIBLE );
            map_id++;
        }
    }
}

/**
    Handels the menu actions for all DockWidgets.
*/
void DockManager::dockAction( int id )
{
    switch( action_mapper[id].second )
    {
        case CLOSE_ALL:

            for( QVector<DockWidget *>::iterator it = dockwidget_container.begin(); it != dockwidget_container.end(); it++ )
            {
                ( *it )->setVisible( false );
            }

            break;

        case CLOSE:

            action_mapper[id].first->setVisible( false );

            break;

        case SET_VISIBLE:

            action_mapper[id].first->setVisible( true );

            break;

        case PLACE_LEFT:
            ui_mainwindow->removeDockWidget( action_mapper[id].first );
            action_mapper[id].first->setVisible( true );
            ui_mainwindow->addDockWidget( Qt::LeftDockWidgetArea, action_mapper[id].first );
            break;

        case PLACE_RIGHT:
            ui_mainwindow->removeDockWidget( action_mapper[id].first );
            action_mapper[id].first->setVisible( true );
            ui_mainwindow->addDockWidget( Qt::RightDockWidgetArea, action_mapper[id].first );
            break;

        case PLACE_TOP:
            ui_mainwindow->removeDockWidget( action_mapper[id].first );
            action_mapper[id].first->setVisible( true );
            ui_mainwindow->addDockWidget( Qt::TopDockWidgetArea, action_mapper[id].first );
            break;

        case PLACE_BOTTOM:
            ui_mainwindow->removeDockWidget( action_mapper[id].first );
            action_mapper[id].first->setVisible( true );
            ui_mainwindow->addDockWidget( Qt::BottomDockWidgetArea, action_mapper[id].first );
            break;
    }

    updateDockMenu();
}

QPair<bool, DockWidget *> DockManager::getDockByTitle( const QString &title )
{
    for( QVector<DockWidget *>::iterator it( dockwidget_container.begin() ); it != dockwidget_container.end(); it++ )
    {
        if( ( *it )->windowTitle() == title )
        {
            return qMakePair( true, *it );
        }
    }

    return qMakePair( false, static_cast<DockWidget *>( NULL ) );
}
