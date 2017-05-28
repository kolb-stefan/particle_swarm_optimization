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

#include "graphwidget.h"

GraphWidget::GraphWidget( MainWindow *mw, QWidget *parent, Qt::WindowFlags f ) : QWidget( parent, f ), ui_mainwindow( mw )
{
    QBoxLayout *layout = new QBoxLayout( QBoxLayout::TopToBottom, this );
    setLayout( layout );

    ui_graph = new QwtPlot( QwtText( "Graph" ), this );
    ui_graph->setAxisTitle( QwtPlot::xBottom, "Iterations" );
    ui_graph->setAxisTitle( QwtPlot::yLeft, "Fitness" );
    ui_graph->insertLegend( new QwtLegend() );

    layout->addWidget( ui_graph );
    best_fitness_curve = new QwtPlotCurve( "best" );
    best_fitness_curve->setPen( QPen( QColor( 255, 0, 0 ) ) );
    best_fitness_curve->attach( ui_graph );

    average_fitness_curve = new QwtPlotCurve( "average" );
    average_fitness_curve->setPen( QPen( QColor( 0, 0, 255 ) ) );
    average_fitness_curve->attach( ui_graph );
}

GraphWidget::~GraphWidget()
{

}

void GraphWidget::updateGraph()
{
    if( ui_mainwindow->getSwarm()->getIterationStep() == 0 )
    {
        iterations_data.clear();
        best_fitness_data.clear();
        average_fitness_data.clear();
    }
    else
    {
        iterations_data.push_back( ui_mainwindow->getSwarm()->getIterationStep() );
        best_fitness_data.push_back( ui_mainwindow->getSwarm()->getBestFitness() );
        average_fitness_data.push_back( ui_mainwindow->getSwarm()->getAverageFitness() );
    }

    best_fitness_curve->setSamples( &*iterations_data.begin(), &*best_fitness_data.begin(), iterations_data.size() );
    average_fitness_curve->setSamples( &*iterations_data.begin(), &*average_fitness_data.begin(), iterations_data.size() );
    ui_graph->replot();
}
