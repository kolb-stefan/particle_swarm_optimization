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

#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include "mainwindow.h"

class GraphWidget : public QWidget
{
        Q_OBJECT
    public:
        GraphWidget( MainWindow *mw, QWidget *parent = 0, Qt::WindowFlags f = 0 );
        ~GraphWidget();

    public slots:
        void updateGraph();

    protected:
        MainWindow              *ui_mainwindow;

        QwtPlot                 *ui_graph;
        std::vector<double>     iterations_data;
        QwtPlotCurve            *best_fitness_curve;
        std::vector<double>     best_fitness_data;
        QwtPlotCurve            *average_fitness_curve;
        std::vector<double>     average_fitness_data;
};

#endif // GRAPHWIDGET_H
