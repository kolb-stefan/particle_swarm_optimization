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

#include "dockwidget.h"

DockWidget::DockWidget( const QString &title, QWidget *parent, Qt::WindowFlags flags )
    : QDockWidget( title, parent, flags )
{

}

DockWidget::DockWidget( QWidget *parent, Qt::WindowFlags flags )
    : QDockWidget( parent, flags )
{

}

DockWidget::~DockWidget()
{

}

void DockWidget::closeEvent( QCloseEvent *event )
{
    setHidden( true );
    emit closed();
}

bool DockWidget::close()
{
    setHidden( true );
    return QWidget::close();
}

void DockWidget::show()
{
    setHidden( false );
    setEnabled( true );
}
