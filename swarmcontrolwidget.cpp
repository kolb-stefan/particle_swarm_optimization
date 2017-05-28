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

#include "swarmcontrolwidget.h"
#include "functionoptionswidget.h"
#include "particleviewwidget.h"
#include "graphwidget.h"

SwarmControlWidget::SwarmControlWidget( MainWindow *mw, QWidget *parent, Qt::WindowFlags f ) : QWidget( parent, f ), ui_mainwindow( mw )
{
    double min = -100, max = 100;
    unsigned int row = 0;

    QGridLayout *layout = new QGridLayout();

    {
        QBoxLayout *bl  = new QBoxLayout( QBoxLayout::TopToBottom );
        bl->addLayout( layout );
        bl->addStretch();
        setLayout( bl );
    }


    layout->addWidget( new QLabel( "particles:" ), row, 0 );
    ui_particle_number = new QSpinBox( this );
    ui_particle_number->setRange( 0, 99999 );
    ui_particle_number->setValue( 20 );
    layout->addWidget( ui_particle_number, row, 1 );

    row++;
    {
        QBoxLayout *bl  = new QBoxLayout( QBoxLayout::LeftToRight );
        layout->addWidget( new QLabel( "find: " ), row, 0 );

        ui_findmin = new QRadioButton( "min", this );
        connect( ui_findmin, SIGNAL( toggled( bool ) ), this, SLOT( setFindMinMax() ) );

        bl->addWidget( ui_findmin );
        ui_findmax = new QRadioButton( "max", this );
        ui_findmax->setChecked( true );
        connect( ui_findmax, SIGNAL( toggled( bool ) ), this, SLOT( setFindMinMax() ) );
        bl->addWidget( ui_findmax );
        layout->addLayout( bl, row, 1 );

        setFindMinMax();
    }

    row++;
    {
        layout->addWidget( new QLabel( "max velocity:" ), row, 0 );
        ui_max_velocity = new QDoubleSpinBox( this );
        ui_max_velocity->setMinimumWidth( 10 );
        ui_max_velocity->setRange( 0, std::numeric_limits<double>::max() );
        ui_max_velocity->setDecimals( 3 );
        ui_max_velocity->setSingleStep( 0.1 );
        ui_max_velocity->setValue( 10.0 );

        connect( ui_max_velocity, SIGNAL( valueChanged( double ) ), this, SLOT( setMaxVelocity() ) );
        layout->addWidget( ui_max_velocity, row, 1 );
    }

    row++;
    {
        QFrame *f = new QFrame( this );
        f->setFrameShape( QFrame::HLine );
        layout->addWidget( f, row, 0, 1, 2 );
    }

    row++;
    {
        layout->addWidget( new QLabel( "mode:" ), row, 0 );
        ui_methode_switch = new QComboBox( this );
        ui_methode_switch->addItem( "global best" );
        ui_methode_switch->addItem( "global&local best" );
        connect( ui_methode_switch, SIGNAL( activated( int ) ), this, SLOT( setComputationMode( int ) ) );
        connect( ui_methode_switch, SIGNAL( activated( int ) ), this, SLOT( changeComputationModeLayout( int ) ) );
        layout->addWidget( ui_methode_switch, row, 1 );
    }

    parameter_widgetlist.resize( 2 );

    row++;
    {
        QWidget *current;

        row++;
        {
            current = new QLabel( "parameter c1:" );
            layout->addWidget( current, row, 0 );
            parameter_widgetlist[0].push_back( current );
            parameter_widgetlist[1].push_back( current );

            ui_parameter_c1 = new QDoubleSpinBox( this );
            ui_parameter_c1->setRange( min, max );
            ui_parameter_c1->setSingleStep( 0.01 );
            ui_parameter_c1->setValue( 2 );
            connect( ui_parameter_c1, SIGNAL( valueChanged( double ) ), this, SLOT( setSwarmParameter() ) );
            layout->addWidget( ui_parameter_c1, row, 1 );
            parameter_widgetlist[0].push_back( ui_parameter_c1 );
            parameter_widgetlist[1].push_back( ui_parameter_c1 );
        }

        row++;
        {
            current = new QLabel( "parameter c2:" );
            layout->addWidget( current, row, 0 );
            parameter_widgetlist[0].push_back( current );
            parameter_widgetlist[1].push_back( current );

            ui_parameter_c2 = new QDoubleSpinBox( this );
            ui_parameter_c2->setRange( min, max );
            ui_parameter_c2->setSingleStep( 0.01 );
            ui_parameter_c2->setValue( 2 );
            connect( ui_parameter_c2, SIGNAL( valueChanged( double ) ), this, SLOT( setSwarmParameter() ) );
            layout->addWidget( ui_parameter_c2, row, 1 );
            parameter_widgetlist[0].push_back( ui_parameter_c2 );
            parameter_widgetlist[1].push_back( ui_parameter_c2 );
        }

        row++;
        {
            current = new QLabel( "parameter c3:" );
            layout->addWidget( current, row, 0 );
            parameter_widgetlist[1].push_back( current );

            ui_parameter_c3 = new QDoubleSpinBox( this );
            ui_parameter_c3->setRange( min, max );
            ui_parameter_c3->setSingleStep( 0.01 );
            ui_parameter_c3->setValue( 2 );
            connect( ui_parameter_c3, SIGNAL( valueChanged( double ) ), this, SLOT( setSwarmParameter() ) );
            layout->addWidget( ui_parameter_c3, row, 1 );
            parameter_widgetlist[1].push_back( ui_parameter_c3 );
        }

        row++;
        {
            current = new QLabel( "parameter w:" );
            layout->addWidget( current, row, 0 );
            parameter_widgetlist[0].push_back( current );
            parameter_widgetlist[1].push_back( current );

            ui_parameter_w = new QDoubleSpinBox( this );
            ui_parameter_w->setRange( 0, max );
            ui_parameter_w->setSingleStep( 0.01 );
            ui_parameter_w->setValue( 1.0 );
            connect( ui_parameter_w, SIGNAL( valueChanged( double ) ), this, SLOT( setSwarmParameter() ) );
            layout->addWidget( ui_parameter_w, row, 1 );
            parameter_widgetlist[0].push_back( ui_parameter_w );
            parameter_widgetlist[1].push_back( ui_parameter_w );
        }

        row++;
        {
            current = new QLabel( "parameter r:" );
            layout->addWidget( current, row, 0 );
            parameter_widgetlist[1].push_back( current );

            ui_parameter_neighbour_radius = new QDoubleSpinBox( this );
            ui_parameter_neighbour_radius->setRange( min, max );
            ui_parameter_neighbour_radius->setSingleStep( 0.01 );
            ui_parameter_neighbour_radius->setValue( 1 );
            layout->addWidget( ui_parameter_neighbour_radius, row, 1 );
            connect( ui_parameter_neighbour_radius, SIGNAL( valueChanged( double ) ), this, SLOT( setSwarmParameter() ) );
            parameter_widgetlist[1].push_back( ui_parameter_neighbour_radius );
        }

        setSwarmParameter();
    }

    setComputationMode( 0 );
    changeComputationModeLayout( 0 );

    row++;
    {

        layout->addWidget( new QLabel( "initial alignment:" ), row, 0 );
        ui_swarm_create_methode = new QComboBox( this );
        ui_swarm_create_methode->addItem( "random" );
        ui_swarm_create_methode->addItem( "grid" );
        layout->addWidget( ui_swarm_create_methode, row, 1 );
    }

    row++;
    {
        ui_abort_criterion = new QCheckBox( "abort iterations:", this );
        ui_abort_criterion->setCheckable( true );
        ui_abort_criterion->setChecked( true );
        connect( ui_abort_criterion, SIGNAL( toggled( bool ) ), this, SLOT( changeAbortCriterion( bool ) ) );

        layout->addWidget( ui_abort_criterion, row, 0 );

        ui_abort_criterion_iterations = new QSpinBox( this );
        ui_abort_criterion_iterations->setRange( 1, 999 );
        ui_abort_criterion_iterations->setSingleStep( 1 );
        ui_abort_criterion_iterations->setValue( 1 );
        connect( ui_abort_criterion_iterations, SIGNAL( valueChanged( int ) ), this, SLOT( setSwarmAbortCriterionIterations( int ) ) );
        layout->addWidget( ui_abort_criterion_iterations, row, 1 );
    }
    changeAbortCriterion( true );

    row++;
    {
        ui_auto_velocity_switch = new QCheckBox( "auto adjust velocity:", this );
        ui_auto_velocity_switch->setCheckable( true );
        connect( ui_auto_velocity_switch, SIGNAL( toggled( bool ) ), this, SLOT( changeAutoVelocity( bool ) ) );
        layout->addWidget( ui_auto_velocity_switch, row,  0 );

        QBoxLayout *bl  = new QBoxLayout( QBoxLayout::LeftToRight );
        ui_auto_velocity_min = new QDoubleSpinBox( this );
        ui_auto_velocity_min->setMinimumWidth( 10 );
        ui_auto_velocity_min->setMinimum( ui_max_velocity->minimum() );
        ui_auto_velocity_min->setMaximum( ui_max_velocity->maximum() );
        ui_auto_velocity_min->setSingleStep( 0.01 );
        ui_auto_velocity_min->setValue( 0.5 );
        connect( ui_auto_velocity_min, SIGNAL( valueChanged( double ) ), this, SLOT( setSwarmAutoVelocityMin( double ) ) );
        bl->addWidget( ui_auto_velocity_min );

        ui_auto_velocity_current_status = new QLabel( "0.0" );
        bl->addWidget( ui_auto_velocity_current_status );
        layout->addLayout( bl, row, 1 );
    }

    ui_auto_velocity_switch->setChecked( false );
    changeAutoVelocity( false );

    row++;
    {
        QFrame *f = new QFrame( this );
        f->setFrameShape( QFrame::HLine );
        layout->addWidget( f, row, 0, 1, 2 );
    }

    row++;
    {
        ui_iterations_desctiption = new QLabel( "Iterations: " );
        layout->addWidget( ui_iterations_desctiption, row, 0 );
        ui_iterations_status = new QLabel( "0" );
        layout->addWidget( ui_iterations_status, row, 1 );
    }

    row++;
    {
        ui_fitness_desctiption = new QLabel( "fitness: " );
        layout->addWidget( ui_fitness_desctiption, row, 0 );
        ui_fitness_status = new QLabel( "0.0" );
        layout->addWidget( ui_fitness_status, row, 1 );
    }

    row++;
    {
        QBoxLayout *bl  = new QBoxLayout( QBoxLayout::LeftToRight );

        ui_position_description = new QLabel( "position: " );
        layout->addWidget( ui_position_description, row, 0 );
        ui_position_x_status = new QLabel( "0.0" );
        bl->addWidget( ui_position_x_status );

        ui_position_y_status = new QLabel( "0.0" );
        bl->addWidget( ui_position_y_status );

        layout->addLayout( bl, row, 1 );
    }

    row++;
    {
        int current_timeout = 300;
        QBoxLayout *bl  = new QBoxLayout( QBoxLayout::LeftToRight );

        ui_refresh_time_description = new QLabel( "Refresh Time:" );
        layout->addWidget( ui_refresh_time_description, row, 0 );

        ui_timeout_slider = new QSlider( Qt::Horizontal, this );
        ui_timeout_slider->setRange( 0, 1000 );
        ui_timeout_slider->setSliderPosition( current_timeout );

        connect( ui_timeout_slider, SIGNAL( valueChanged( int ) ), this, SLOT( setTimerTimeout( int ) ) );

        bl->addWidget( ui_timeout_slider );

        ui_refresh_time_status = new QLabel( "" );
        bl->addWidget( ui_refresh_time_status );

        layout->addLayout( bl, row, 1 );

        setTimerTimeout( current_timeout );
    }

    row++;
    {
        ui_create_swarm = new QPushButton( "Create Swarm", this );
        connect( ui_create_swarm, SIGNAL( clicked() ), this, SLOT( createSwarm() ) );
        layout->addWidget( ui_create_swarm, row, 0 );

        ui_start_swarm = new QPushButton( "Start", this );
        connect( ui_start_swarm, SIGNAL( clicked() ), this, SLOT( toggleComputation() ) );
        layout->addWidget( ui_start_swarm, row, 1 );
    }

    disableTimer();
}

SwarmControlWidget::~SwarmControlWidget()
{

}

void SwarmControlWidget::setFindMinMax()
{
    Swarm<Function> *swarm = ui_mainwindow->getSwarm();

    if( ui_findmin->isChecked() )
    {
        swarm->setCompare( true );
    }
    else if( ui_findmax->isChecked() )
    {
        swarm->setCompare( false );
    }
}

void SwarmControlWidget::setMaxVelocity()
{
    ui_mainwindow->getSwarm()->setMaxVelocity( ui_max_velocity->value() );
}

void SwarmControlWidget::setComputationMode( int index )
{
    if( index != 0 && index != 1 ) {return;}

    if( index == 0 ) // GLOBAL_BEST
    {
        ui_mainwindow->getSwarm()->setComputationMethode( Swarm<Function>::GLOBAL_BEST );
    }
    else if( index == 1 ) // GLOBAL_LOCAL_BEST
    {
        ui_mainwindow->getSwarm()->setComputationMethode( Swarm<Function>::GLOBAL_LOCAL_BEST );
    }
}

void SwarmControlWidget::changeComputationModeLayout( int index )
{
    if( index != 0 && index != 1 ) {return;}

    if( index < static_cast<int>( parameter_widgetlist.size() ) )
    {
        for( QVector<QVector<QWidget *> >::iterator it1( parameter_widgetlist.begin() ); it1 != parameter_widgetlist.end(); it1++ )
        {
            for( QVector<QWidget *>::iterator it2( it1->begin() ); it2 != it1->end(); it2++ )
            {
                ( *it2 )->setVisible( false );
            }
        }

        for( QVector<QWidget *>::iterator it( parameter_widgetlist[index].begin() ); it != parameter_widgetlist[index].end(); it++ )
        {
            ( *it )->setVisible( true );
        }
    }
}

void SwarmControlWidget::setSwarmParameter()
{
    Swarm<Function> *swarm = ui_mainwindow->getSwarm();
    swarm->setParameterC1( ui_parameter_c1->value() );
    swarm->setParameterC2( ui_parameter_c2->value() );
    swarm->setParameterC3( ui_parameter_c3->value() );
    swarm->setParameterW( ui_parameter_w->value() );
    swarm->setNeighbourRadius( ui_parameter_neighbour_radius->value() );
}

void SwarmControlWidget::createSwarm()
{
    ui_iterations_status->setText( "0" );
    ui_fitness_status->setText( "0.0" );
    ui_position_x_status->setText( "0.0" );
    ui_position_y_status->setText( "0.0" );

    disableTimer();

    try
    {
        setMaxVelocity();
        QString num;
        num.setNum( ui_max_velocity->value() );
        ui_auto_velocity_current_status->setText( num );

        VectorN<double> min( 2 ), max( 2 );

        ui_mainwindow->getFunctionOptionsWidget()->getFunctionRange2D( min, max );

        if( ui_swarm_create_methode->currentIndex() == 0 )
        {
            ui_mainwindow->getSwarm()->createSwarm( ui_particle_number->value(), min, max, true );
        }
        else if( ui_swarm_create_methode->currentIndex() == 1 )
        {
            ui_mainwindow->getSwarm()->createSwarm( ui_particle_number->value(), min, max, false );
        }
        else
        {
            throw RuntimeError( "undefined selection in QComboBox m_swarm_create_methode_combobox!" );
        }

        ui_mainwindow->getSwarm()->findGlobalBest();
    }
    catch( RuntimeError &err )
    {
        ui_mainwindow->showError( err );
    }

    ui_mainwindow->getParticleViewWidget()->updateView();
    ui_mainwindow->getGraphWidget()->updateGraph();

    FunctionViewer *gl = ui_mainwindow->getGLWidget();

    if( gl->isParticleTracingEnabled() )
    {
        gl->getTraceParticleContainer().clear();
    }

    gl->updateGL();
}

void SwarmControlWidget::changeAbortCriterion( bool checked )
{
    ui_abort_criterion_iterations->setEnabled( checked );
    ui_mainwindow->getSwarm()->setCheckAbortCriterion( checked );
}

void SwarmControlWidget::setSwarmAbortCriterionIterations( int value )
{
    ui_mainwindow->getSwarm()->setAbortCriterionIterations( value );
}

void SwarmControlWidget::changeAutoVelocity( bool checked )
{
    ui_auto_velocity_min->setEnabled( checked );
    ui_auto_velocity_current_status->setEnabled( checked );

    ui_mainwindow->getSwarm()->setAutoVelocity( checked );
}

void SwarmControlWidget::setSwarmAutoVelocityMin( double value )
{
    ui_mainwindow->getSwarm()->setMinVelocity( value );
}

void SwarmControlWidget::toggleComputation()
{
    if( ui_mainwindow->getTimer()->isActive() )
    {
        disableTimer();
    }
    else
    {
        enableTimer();
    }
}

void SwarmControlWidget::enableTimer()
{
    ui_mainwindow->getTimer()->start();
    ui_start_swarm->setText( "Stop" );
}

void SwarmControlWidget::disableTimer()
{
    ui_mainwindow->getTimer()->stop();
    ui_start_swarm->setText( "Start" );
}

void SwarmControlWidget::setTimerTimeout( int timeout )
{
    QString num;
    num.setNum( timeout );
    num += QString( " ms" );
    ui_refresh_time_status->setText( num );

    ui_mainwindow->getTimer()->setInterval( timeout );
}

void SwarmControlWidget::showUsedIterations( int iterations )
{
    QString num;
    num.setNum( iterations );
    ui_iterations_status->setText( num );
}

void SwarmControlWidget::showBestPartileFitness( double fitness )
{
    QString num;
    num.setNum( fitness );
    ui_fitness_status->setText( num );
}

void SwarmControlWidget::showCurrentBestFoundPosition( double x, double y )
{
    QString num;
    num.setNum( x );
    ui_position_x_status->setText( num );

    num.setNum( y );
    ui_position_y_status->setText( num );
}

void SwarmControlWidget::showCurrentMaxVelocity( double value )
{
    QString num;
    num.setNum( value );
    ui_auto_velocity_current_status->setText( num );
}

void SwarmControlWidget::changeApplicationMode( PSOMode mode )
{
    switch( mode )
    {

        case PSOMode3DView:
            ui_abort_criterion->setEnabled( true );

            ui_fitness_desctiption->setEnabled( true );
            ui_fitness_status->setEnabled( true );

            ui_iterations_desctiption->setEnabled( true );
            ui_iterations_status->setEnabled( true );

            ui_position_description->setEnabled( true );
            ui_position_x_status->setEnabled( true );
            ui_position_y_status->setEnabled( true );

            ui_refresh_time_description->setEnabled( true );
            ui_timeout_slider->setEnabled( true );
            ui_refresh_time_status->setEnabled( true );

            ui_start_swarm->setEnabled( true );
            ui_create_swarm->setEnabled( true );
            break;

        case PSOModeVariation:
            setSwarmParameter();
            setFindMinMax();


            ui_abort_criterion->setEnabled( false );
            ui_abort_criterion->setChecked( true );
            changeAbortCriterion( true );

            ui_fitness_desctiption->setEnabled( false );
            ui_fitness_status->setEnabled( false );

            ui_iterations_desctiption->setEnabled( false );
            ui_iterations_status->setEnabled( false );

            ui_position_description->setEnabled( false );
            ui_position_x_status->setEnabled( false );
            ui_position_y_status->setEnabled( false );

            ui_refresh_time_description->setEnabled( false );
            ui_timeout_slider->setEnabled( false );
            ui_refresh_time_status->setEnabled( false );


            ui_start_swarm->setEnabled( false );
            ui_create_swarm->setEnabled( false );
            break;
    }
}

void SwarmControlWidget::setParticleNumber( unsigned int number )
{
    ui_particle_number->setValue( number );
}

unsigned int SwarmControlWidget::getParticleNumber()
{
    return ui_particle_number->value();
}

bool SwarmControlWidget::checkFindModeMin()
{
    return ui_findmin->isChecked();
}

bool SwarmControlWidget::isAutoVelocityUsed()
{
    return ui_auto_velocity_switch->isChecked();
}

bool SwarmControlWidget::isCreationModeRandom()
{
    return ui_swarm_create_methode->currentIndex() == 0;
}

unsigned int SwarmControlWidget::getCurrentTimerTimeout()
{
    return ui_timeout_slider->value();
}
