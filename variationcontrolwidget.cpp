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

#include "variationcontrolwidget.h"

#include "swarmcontrolwidget.h"
#include <limits>

VariationControlWidget::VariationControlWidget( MainWindow *mw, QWidget *parent, Qt::WindowFlags f ): QWidget( parent, f ), ui_mainwindow( mw )
{
    variable_names["particle"] = "particle";
    variable_names["c1"] = "c1";
    variable_names["c2"] = "c2";
    variable_names["c3"] = "c3";
    variable_names["w"] = "w";
    variable_names["radius"] = "radius";
    variable_names["max velocity"] = "max velocity";

    int row = 0;
    QBoxLayout *tl = new QBoxLayout( QBoxLayout::TopToBottom );
    setLayout( tl );
    QGridLayout *layout = new QGridLayout;

    tl->addLayout( layout );
    tl->addStretch();

    layout->addWidget( new QLabel( "Variation:" ), row, 0 );

    ui_variable_select = new QComboBox( this );
    ui_variable_select->addItem( variable_names["particle"] );
    ui_variable_select->addItem( variable_names["c1"] );
    ui_variable_select->addItem( variable_names["c2"] );
    ui_variable_select->addItem( variable_names["c3"] );
    ui_variable_select->addItem( variable_names["w"] );
    ui_variable_select->addItem( variable_names["radius"] );
    ui_variable_select->addItem( variable_names["max velocity"] );

    connect( ui_variable_select, SIGNAL( activated( const QString & ) ), this, SLOT( changeVariation( const QString & ) ) );
    layout->addWidget( ui_variable_select, row, 1, 1, 2 );

    row++;
    layout->addWidget( new QLabel( "from:" ), row, 0 );
    layout->addWidget( new QLabel( "to:" ), row, 1 );
    layout->addWidget( new QLabel( "step:" ), row, 2 );

    row++;
    ui_from_value = new QDoubleSpinBox( this );
    layout->addWidget( ui_from_value, row, 0 );
    ui_to_value = new QDoubleSpinBox( this );
    layout->addWidget( ui_to_value, row, 1 );
    ui_step = new QDoubleSpinBox( this );
    layout->addWidget( ui_step, row, 2 );

    row++;
    layout->addWidget( new QLabel( "average num:" ), row, 0 );
    ui_average_number = new QSpinBox( this );
    ui_average_number->setRange( 1, 9999 );
    layout->addWidget( ui_average_number, row, 1, 1, 2 );

    row++;
    ui_start = new QPushButton( "Start Statistic" );
    connect( ui_start, SIGNAL( clicked() ), this, SLOT( startVariation() ) );
    layout->addWidget( ui_start, row, 0, 1, 3 );

    changeVariation( variable_names["particle"] );
}

VariationControlWidget::~VariationControlWidget()
{

}

void VariationControlWidget::changeVariation( const QString &str )
{

    ui_start->setEnabled( true );


    ui_step->setDecimals( 2 );
    ui_from_value->setDecimals( 2 );
    ui_to_value->setDecimals( 2 );

    double dstep = 0.1, dstepstep = 0.01;

    if( str == variable_names["particle"] )
    {
        ui_step->setDecimals( 0 );
        ui_from_value->setDecimals( 0 );
        ui_to_value->setDecimals( 0 );

        ui_step->setSingleStep( 1.0 );
        ui_step->setRange( 1.0, std::numeric_limits<int>::max() );
        ui_step->setValue( 1.0 );
        ui_step->setDecimals( 0 );

        ui_from_value->setSingleStep( 1.0 );
        ui_from_value->setRange( 1, std::numeric_limits<int>::max() );
        ui_from_value->setValue( 1 );

        ui_to_value->setSingleStep( 1.0 );
        ui_to_value->setRange( 1, std::numeric_limits<int>::max() );
        ui_to_value->setValue( 100 );

        ui_mainwindow->getVariationPlotWidget()->setAxisTitle( QwtPlot::xBottom, "particles" );
        ui_mainwindow->getVariationPlotWidget()->setAxisTitle( QwtPlot::yLeft, "iterations" );
//         m_statistic_variable_qwtplotcurve->setTitle( "iterations" );
    }
    else if( str == variable_names["c1"] )
    {
        ui_step->setSingleStep( dstepstep );
        ui_step->setRange( 0.0, 100.0 );
        ui_step->setValue( dstep );

        ui_from_value->setSingleStep( dstep );
        ui_from_value->setRange( std::numeric_limits<double>::min(), std::numeric_limits<double>::max() );
        ui_from_value->setValue( 0.0 );

        ui_to_value->setSingleStep( dstep );
        ui_to_value->setRange( std::numeric_limits<double>::min(), std::numeric_limits<double>::max() );
        ui_to_value->setValue( 4.0 );

        ui_mainwindow->getVariationPlotWidget()->setAxisTitle( QwtPlot::xBottom, "c1" );
        ui_mainwindow->getVariationPlotWidget()->setAxisTitle( QwtPlot::yLeft, "iterations" );
//         m_statistic_variable_qwtplotcurve->setTitle( "iterations" );
    }
    else if( str == variable_names["c2"] )
    {
        ui_step->setSingleStep( dstepstep );
        ui_step->setRange( 0.0, 100.0 );
        ui_step->setValue( dstep );

        ui_from_value->setSingleStep( dstep );
        ui_from_value->setRange( std::numeric_limits<double>::min(), std::numeric_limits<double>::max() );
        ui_from_value->setValue( 0.0 );

        ui_to_value->setSingleStep( dstep );
        ui_to_value->setRange( std::numeric_limits<double>::min(), std::numeric_limits<double>::max() );
        ui_to_value->setValue( 4.0 );

        ui_mainwindow->getVariationPlotWidget()->setAxisTitle( QwtPlot::xBottom, "c2" );
        ui_mainwindow->getVariationPlotWidget()->setAxisTitle( QwtPlot::yLeft, "iterations" );
//         m_statistic_variable_qwtplotcurve->setTitle( "iterations" );
    }
    else if( str == variable_names["c3"] )
    {
        if( ui_mainwindow->getSwarm()->getComputationMethode() != Swarm<Function>::GLOBAL_LOCAL_BEST )
        {
            ui_start->setEnabled( false );
        }

        ui_step->setSingleStep( dstepstep );
        ui_step->setRange( 0.0, 100.0 );
        ui_step->setValue( dstep );

        ui_from_value->setSingleStep( dstep );
        ui_from_value->setRange( std::numeric_limits<double>::min(), std::numeric_limits<double>::max() );
        ui_from_value->setValue( 0.0 );

        ui_to_value->setSingleStep( dstep );
        ui_to_value->setRange( std::numeric_limits<double>::min(), std::numeric_limits<double>::max() );
        ui_to_value->setValue( 4.0 );

        ui_mainwindow->getVariationPlotWidget()->setAxisTitle( QwtPlot::xBottom, "c3" );
        ui_mainwindow->getVariationPlotWidget()->setAxisTitle( QwtPlot::yLeft, "iterations" );
//         m_statistic_variable_qwtplotcurve->setTitle( "iterations" );
    }
    else if( str == variable_names["w"] )
    {
        ui_step->setSingleStep( dstepstep );
        ui_step->setRange( 0.0, 100.0 );
        ui_step->setValue( dstep );

        ui_from_value->setSingleStep( dstep );
        ui_from_value->setRange( std::numeric_limits<double>::min(), std::numeric_limits<double>::max() );
        ui_from_value->setValue( 0.0 );

        ui_to_value->setSingleStep( dstep );
        ui_to_value->setRange( std::numeric_limits<double>::min(), std::numeric_limits<double>::max() );
        ui_to_value->setValue( 4.0 );

        ui_mainwindow->getVariationPlotWidget()->setAxisTitle( QwtPlot::xBottom, "w" );
        ui_mainwindow->getVariationPlotWidget()->setAxisTitle( QwtPlot::yLeft, "iterations" );
//         m_statistic_variable_qwtplotcurve->setTitle( "iterations" );
    }
    else if( str == variable_names["radius"] )
    {
        if( ui_mainwindow->getSwarm()->getComputationMethode() != Swarm<Function>::GLOBAL_LOCAL_BEST )
        {
            ui_start->setEnabled( false );
        }

        ui_step->setSingleStep( dstepstep );
        ui_step->setRange( 0.0, 100.0 );
        ui_step->setValue( dstep );

        ui_from_value->setSingleStep( dstep );
        ui_from_value->setRange( std::numeric_limits<double>::min(), std::numeric_limits<double>::max() );
        ui_from_value->setValue( 0.0 );

        ui_to_value->setSingleStep( dstep );
        ui_to_value->setRange( std::numeric_limits<double>::min(), std::numeric_limits<double>::max() );
        ui_to_value->setValue( 4.0 );

        ui_mainwindow->getVariationPlotWidget()->setAxisTitle( QwtPlot::xBottom, "radius" );
        ui_mainwindow->getVariationPlotWidget()->setAxisTitle( QwtPlot::yLeft, "iterations" );
    }
    else if( str == variable_names["max velocity"] )
    {
        ui_step->setSingleStep( dstepstep );
        ui_step->setRange( 0.0, 1000.0 );
        ui_step->setValue( dstep );

        ui_from_value->setSingleStep( dstep );
        ui_from_value->setRange( std::numeric_limits<double>::min(), std::numeric_limits<double>::max() );
        ui_from_value->setValue( 1.0 );

        ui_to_value->setSingleStep( dstep );
        ui_to_value->setRange( std::numeric_limits<double>::min(), std::numeric_limits<double>::max() );
        ui_to_value->setValue( 10.0 );

        ui_mainwindow->getVariationPlotWidget()->setAxisTitle( QwtPlot::xBottom, "max velocity" );
        ui_mainwindow->getVariationPlotWidget()->setAxisTitle( QwtPlot::yLeft, "iterations" );
    }
}

void VariationControlWidget::startVariation()
{
    try
    {
        ui_mainwindow->getSwarm()->clear();

        if( ui_mainwindow->getTimer()->isActive() )
        {
            disableTimer();
            return;
        }

        if( ui_from_value->value() >= ui_to_value->value() )
        {
            throw RuntimeError( "from value should be less than to value!!" );
        }

        ui_mainwindow->getSwarmControlWidget()->setSwarmParameter();
        ui_mainwindow->getSwarmControlWidget()->setFindMinMax();
        ui_mainwindow->getSwarmControlWidget()->setMaxVelocity();

        if( ui_mainwindow->getSwarm()->getComputationMethode() == Swarm<Function>::GLOBAL_BEST && ui_variable_select->currentText() == variable_names["radius"] )
        {
            throw RuntimeError( "wrong settings for radius" );
        }

        ui_mainwindow->getTimer()->setInterval( 0 );
        enableTimer();

        old_from_value =  ui_from_value->value();
        ui_mainwindow->clearVariationGraphData();
    }
    catch( RuntimeError &err )
    {
        ui_mainwindow->showError( err );
    }
}

unsigned int VariationControlWidget::getAverageNumber()
{
    return ui_average_number->value();
}

double VariationControlWidget::getFromValue()
{
    return ui_from_value->value();
}

double VariationControlWidget::getToValue()
{
    return ui_to_value->value();
}

double VariationControlWidget::getOldFromValue()
{
    return old_from_value;
}

double VariationControlWidget::getStepValue()
{
    return ui_step->value();
}

void VariationControlWidget::setFromValue( double value )
{
    ui_from_value->setValue( value );
}

QMap< QString, QString > &VariationControlWidget::getVariationVariableNames()
{
    return variable_names;
}

void VariationControlWidget::enableTimer()
{
    ui_mainwindow->getTimer()->start();

    ui_variable_select->setEnabled( false );
    ui_from_value->setEnabled( false );
    ui_to_value->setEnabled( false );
    ui_step->setEnabled( false );
    ui_average_number->setEnabled( false );
    ui_start->setText( "Stop Variation" );
}

void VariationControlWidget::disableTimer()
{
    ui_mainwindow->getTimer()->stop();

    ui_start->setText( "Start Variation" );

    ui_variable_select->setEnabled( true );
    ui_from_value->setEnabled( true );
    ui_to_value->setEnabled( true );
    ui_step->setEnabled( true );

    ui_average_number->setEnabled( true );
    ui_from_value->setValue( old_from_value );

    ui_mainwindow->getSwarmControlWidget()->setEnabled( true );
}

QString VariationControlWidget::getCurrentlyUsedVariable()
{
    return ui_variable_select->currentText();
}
