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

#ifndef PARTICLEVIEWWIDGET_H
#define PARTICLEVIEWWIDGET_H

#include <QWidget>
#include "mainwindow.h"


class ParticleViewWidget : public QWidget
{
        Q_OBJECT
    public:
        ParticleViewWidget( MainWindow *mw, QWidget *parent = 0, Qt::WindowFlags f = 0 );
        ~ParticleViewWidget();

    public slots:

        void updateView();
    protected:
        MainWindow          *ui_mainwindow;
        QStandardItemModel  *particle_model;
};

#endif // PARTICLEVIEWWIDGET_H
