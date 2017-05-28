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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Qt>
#include <QtGui>

#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_text.h>
#include <qwt/qwt_legend.h>

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <algorithm>

#include "dockmanager.h"
#include "functionviewer.h"
#include "functionmanagerdialog.h"
#include "functioneditdialog.h"
#include "error.h"

class SwarmControlWidget;
class FunctionOptionsWidget;
class VariationControlWidget;
class ParticleViewWidget;
class GraphWidget;

enum PSOMode { PSOMode3DView, PSOModeVariation };

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
        void showError( const RuntimeError &err );

        Swarm<Function> *getSwarm();
        FunctionViewer *getGLWidget();
        QwtPlot *getVariationPlotWidget();

        SwarmControlWidget *getSwarmControlWidget();
        FunctionOptionsWidget *getFunctionOptionsWidget();
        VariationControlWidget *getStatisticControlWidget();
        ParticleViewWidget *getParticleViewWidget();
        GraphWidget *getGraphWidget();

        QTimer *getTimer();

        void clearVariationGraphData();

        PSOMode getCurrentApplicationMode() const;
        void setApplicationMode( PSOMode mode );

    public slots:

        void changeVariationMaxIterations();
        void particleNumberChanged();
        void  timerTimeOut();

        void changeApplicationMode( QAction *action );

        void changeGLWireframe( bool w );
        void changeGLPoint( bool w );

    protected:
        void computeNextStep();

        FunctionViewer              *ui_functionviewer;
        QMap<QString, QAction *>      menu_actions_container;

        SwarmControlWidget          *ui_swarm_control;
        FunctionOptionsWidget       *ui_function_options;
        VariationControlWidget      *ui_variation_control;
        ParticleViewWidget          *ui_particle_view;
        GraphWidget                 *ui_graph_widget;

        QTimer                      *timer;

        QwtPlot                     *variation_graph;
        QwtPlotCurve                *variation_variable_curve;
        QwtPlotCurve                *variation_fitness_curve;

        std::vector<double>         variation_variables_data;
        std::vector<double>         variation_iterations_data;
        std::vector<double>         variation_fitness_data;

        DockManager                 *ui_dockmanager;
        QMap<QString, DockWidget *>   ui_dockwidgets;

        size_t                      variation_max_iterations;
        PSOMode                     application_mode;

        Swarm<Function>             swarm;
};

#endif
