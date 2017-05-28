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

#include "mainwindow.h"
#include "swarmcontrolwidget.h"
#include "functionoptionswidget.h"
#include "variationcontrolwidget.h"
#include "particleviewwidget.h"
#include "graphwidget.h"

MainWindow::MainWindow( QWidget *parent, Qt::WindowFlags flags )
    : QMainWindow( parent, flags )
{
    variation_max_iterations = 10000;
    swarm.setDimension( 2 );

    setWindowTitle( "Particle Swarm Optimization" );
    setDockOptions( QMainWindow::ForceTabbedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::AnimatedDocks | QMainWindow::AllowTabbedDocks );

    {
        QWidget *widget = new QWidget( this );
        QBoxLayout *layout = new QBoxLayout( QBoxLayout::TopToBottom, widget );

        ui_functionviewer = new FunctionViewer( this );
        ui_functionviewer->setSwarm( &swarm );
        connect( ui_functionviewer, SIGNAL( particleNumberChanged() ), this, SLOT( particleNumberChanged() ) );
        layout->addWidget( ui_functionviewer );

        variation_graph = new QwtPlot( QwtText( "Variation" ), this );
        variation_graph->setAxisTitle( QwtPlot::xBottom, "particles" );
        variation_graph->setAxisTitle( QwtPlot::yLeft, "iterations" );
        variation_graph->enableAxis( QwtPlot::yRight );
        variation_graph->setAxisTitle( QwtPlot::yRight, "fitness" );

        variation_graph->insertLegend( new QwtLegend() );
        variation_graph->setHidden( true );
        layout->addWidget( variation_graph );

        variation_variable_curve = new QwtPlotCurve( "iterations" );
        variation_variable_curve->setPen( QPen( QColor( 255, 0, 0 ) ) );
        variation_variable_curve->attach( variation_graph );

        variation_fitness_curve = new QwtPlotCurve( "fitness" );
        variation_fitness_curve->setPen( QPen( QColor( 0, 0, 255 ) ) );
        variation_fitness_curve->attach( variation_graph );
        variation_fitness_curve->setYAxis( QwtPlot::yRight );

        widget->setLayout( layout );
        setCentralWidget( widget );
    }

    timer = new QTimer( this );
    connect( timer, SIGNAL( timeout() ), this, SLOT( timerTimeOut() ) );

    QMenu *mode = menuBar()->addMenu( tr( "&Mode" ) );

    QActionGroup *actiongroup = new QActionGroup( this );
    connect( actiongroup, SIGNAL( triggered( QAction * ) ), this, SLOT( changeApplicationMode( QAction * ) ) );

    menu_actions_container["mode:3d"] = actiongroup->addAction( "&3D" );
    menu_actions_container["mode:3d"]->setCheckable( true );
    menu_actions_container["mode:3d"]->setChecked( true );
    mode->addAction( menu_actions_container["mode:3d"] );

    menu_actions_container["mode:variation"] = actiongroup->addAction( "&Variation" );
    menu_actions_container["mode:variation"]->setCheckable( true );
    mode->addAction( menu_actions_container["mode:variation"] );

    mode->addSeparator();
    menu_actions_container["mode:exit"] = mode->addAction( "&Exit", this, SLOT( close() ) );

    QMenu *options = menuBar()->addMenu( tr( "&Options" ) );
    menu_actions_container["options:default view"] = options->addAction( "&Default View", ui_functionviewer, SLOT( setDefaultView() ) );
    menu_actions_container["options:min function color"] = options->addAction( "&Min Function Color", ui_functionviewer, SLOT( changeFunctionMinColor() ) );
    menu_actions_container["options:max function color"] = options->addAction( "&Max Function Color", ui_functionviewer, SLOT( changeFunctionMaxColor() ) );
    menu_actions_container["options:point size"] = options->addAction( "&Point Size", ui_functionviewer, SLOT( changePointSize() ) );
    menu_actions_container["options:point color"] = options->addAction( "&Point Color", ui_functionviewer, SLOT( changePointColor() ) );

    menu_actions_container["options:mini map"] = options->addAction( "&Mini Map" );
    menu_actions_container["options:mini map"]->setCheckable( true );
    menu_actions_container["options:mini map"]->setChecked( true );
    connect( menu_actions_container["options:mini map"], SIGNAL( toggled( bool ) ), ui_functionviewer, SLOT( showMiniMap( bool ) ) );

    menu_actions_container["options:mini map mark best"] = options->addAction( "&Mini Map Mark Best" );
    menu_actions_container["options:mini map mark best"]->setCheckable( true );
    menu_actions_container["options:mini map mark best"]->setChecked( true );
    ui_functionviewer->showMiniMapGlobalBest( true );
    connect( menu_actions_container["options:mini map mark best"], SIGNAL( toggled( bool ) ), ui_functionviewer, SLOT( showMiniMapGlobalBest( bool ) ) );

    menu_actions_container["options:mark best"] = options->addAction( "&Mark Best" );
    menu_actions_container["options:mark best"]->setCheckable( true );
    menu_actions_container["options:mark best"]->setChecked( true );
    ui_functionviewer->showGlobalBest3D( true );
    connect( menu_actions_container["options:mark best"], SIGNAL( toggled( bool ) ), ui_functionviewer, SLOT( showGlobalBest3D( bool ) ) );

#ifdef USE_FTGL
    menu_actions_container["options:show axis"] = options->addAction( "&Show Axis" );
    menu_actions_container["options:show axis"]->setCheckable( true );
    menu_actions_container["options:show axis"]->setChecked( true );
    connect( menu_actions_container["options:show axis"], SIGNAL( toggled( bool ) ), ui_functionviewer, SLOT( showAxis( bool ) ) );

    if( !ui_functionviewer->isFTGLFontLoaded() )
    {
        menu_actions_container["options:show axis"]->setEnabled( false );
        menu_actions_container["options:show axis"]->setChecked( false );
    }

#endif

    menu_actions_container["options:trace particles"] = options->addAction( "&Trace Particles" );
    menu_actions_container["options:trace particles"]->setCheckable( true );
    menu_actions_container["options:trace particles"]->setChecked( false );
    ui_functionviewer->showTraceParticles( false );
    connect( menu_actions_container["options:trace particles"], SIGNAL( toggled( bool ) ), ui_functionviewer, SLOT( showTraceParticles( bool ) ) );

    menu_actions_container["options:gl background color"] = options->addAction( "&GL Background Color", ui_functionviewer, SLOT( changeBackgroundColor() ) );

    menu_actions_container["options:wireframe"] = options->addAction( "&Wireframe" );
    menu_actions_container["options:wireframe"]->setCheckable( true );
    connect( menu_actions_container["options:wireframe"], SIGNAL( toggled( bool ) ), this, SLOT( changeGLWireframe( bool ) ) );

    menu_actions_container["options:points"] = options->addAction( "&Points" );
    menu_actions_container["options:points"]->setCheckable( true );
    connect( menu_actions_container["options:points"], SIGNAL( toggled( bool ) ), this, SLOT( changeGLPoint( bool ) ) );

    options->addSeparator();

    menu_actions_container["options:variation max iterations"] = options->addAction( "&Variation Max Iterations", this, SLOT( changeVariationMaxIterations() ) );
    menu_actions_container["options:variation max iterations"]->setEnabled( false );

    QMenu *dock_menu = new QMenu( tr( "Dock" ) );
    ui_dockmanager = new DockManager( this, dock_menu );
    menuBar()->addMenu( dock_menu );
    setDockOptions( QMainWindow::VerticalTabs );

    ui_dockwidgets["swarm control"] = new DockWidget( tr( "Swarm Control" ) );
    ui_dockmanager->addDock( Qt::LeftDockWidgetArea, ui_dockwidgets["swarm control"] );
    ui_swarm_control = new SwarmControlWidget( this, this );
    ui_dockwidgets["swarm control"]->setWidget( ui_swarm_control );

    ui_dockwidgets["function options"] = new DockWidget( tr( "Function Options" ) );
    ui_dockmanager->addDock( Qt::LeftDockWidgetArea, ui_dockwidgets["function options"] );
    ui_function_options = new FunctionOptionsWidget( this, this );
    ui_dockwidgets["function options"]->setWidget( ui_function_options );

    ui_dockwidgets["variation control"] = new DockWidget( tr( "Variation Coltrol" ) );
    ui_dockmanager->addDock( Qt::LeftDockWidgetArea, ui_dockwidgets["variation control"] );
    ui_variation_control = new VariationControlWidget( this, this );
    ui_dockwidgets["variation control"]->setWidget( ui_variation_control );

    ui_dockwidgets["particle view"] = new DockWidget( tr( "Particle View" ) );
    ui_dockmanager->addDock( Qt::RightDockWidgetArea, ui_dockwidgets["particle view"] );
    ui_particle_view = new ParticleViewWidget( this, this );
    ui_dockwidgets["particle view"]->setWidget( ui_particle_view );
    ui_dockwidgets["particle view"]->close();

    ui_dockwidgets["graph"] = new DockWidget( tr( "Graph" ) );
    ui_dockmanager->addDock( Qt::RightDockWidgetArea, ui_dockwidgets["graph"] );
    ui_graph_widget = new GraphWidget( this, this );
    ui_dockwidgets["graph"]->setWidget( ui_graph_widget );
    ui_dockwidgets["graph"]->close();


    setApplicationMode( PSOMode3DView );

    ui_function_options->setFunction();
}

void MainWindow::changeGLWireframe( bool w )
{
    if( menu_actions_container["options:points"]->isChecked() && w )
    {
        menu_actions_container["options:points"]->setChecked( false );
    }

    ui_functionviewer->setViewModeWireframe( w );
}

void MainWindow::changeGLPoint( bool w )
{
    if( menu_actions_container["options:wireframe"]->isChecked() && w )
    {
        menu_actions_container["options:wireframe"]->setChecked( false );
    }

    ui_functionviewer->setViewModePoint( w );
}

/**
    Handles the timer timeout callback!

    In both application modes the optimization is performed in this function.
    This includes all user interface element updates.
*/
void  MainWindow::timerTimeOut()
{
    switch( application_mode )
    {
        case PSOMode3DView:
            try
            {
                bool check = false;

                if( swarm.m_swarm.empty() )
                {
                    ui_swarm_control->disableTimer();
                }

                if( swarm.getCheckAbortCriterion() )
                {
                    check = !swarm.checkAbortCriterion();
                }
                else
                {
                    swarm.checkAbortCriterion(); //needed for auto velocity see: void Swarm::calculateMaxVelocity()
                    check = true;
                }

                if( check )
                {
                    if( !ui_dockwidgets["particle view"]->isHidden() )
                    {
                        ui_particle_view->updateView();
                    }

                    if( !ui_dockwidgets["graph"]->isHidden() )
                    {
                        ui_graph_widget->updateGraph();
                    }

                    computeNextStep();

                    ui_swarm_control->showUsedIterations( swarm.getIterationStep() );

                    if( swarm.getBestParticle() )
                    {
                        ui_swarm_control->showBestPartileFitness( swarm.getBestParticle()->getBestValue() );
                        ui_swarm_control->showCurrentBestFoundPosition( swarm.getBestParticle()->getBestPosition()[0], swarm.getBestParticle()->getBestPosition()[1] );
                    }

                    if( ui_swarm_control->isAutoVelocityUsed() )
                    {
                        ui_swarm_control->showCurrentMaxVelocity( swarm.getMaxVelocity() );
                    }

                    ui_functionviewer->updateGL();
                }
                else
                {
                    ui_swarm_control->disableTimer();
                }
            }
            catch( RuntimeError &err )
            {
                ui_swarm_control->disableTimer();
                showError( err );
            }

            break;

        case PSOModeVariation:
            if( ui_variation_control->getToValue() <= ui_variation_control->getFromValue() )
            {
                ui_variation_control->disableTimer();
            }
            else
            {
                try
                {
                    size_t average_number = ui_variation_control->getAverageNumber();
                    QString variable = ui_variation_control->getCurrentlyUsedVariable();
                    double average = 0.0;
                    double average_fitness = 0.0;
                    bool random = true;

                    if( ui_swarm_control->isCreationModeRandom() )
                    {
                        random = true;
                    }
                    else
                    {
                        random = false;
                    }

                    for( size_t i = 0; i < average_number; i++ )
                    {
                        ui_swarm_control->setMaxVelocity();
                        unsigned int particle_number = ui_swarm_control->getParticleNumber();

                        if( variable == ui_variation_control->getVariationVariableNames()["particle"] )
                        {
                            particle_number = ui_variation_control->getFromValue();
                        }

                        {
                            VectorN<double> range_min( 1 ), range_max( 1 );
                            ui_function_options->getFunctionRange( range_min, range_max );

                            getSwarm()->createSwarm( particle_number, range_min, range_max, random );
                        }

                        if( variable == ui_variation_control->getVariationVariableNames()["c1"] )
                        {
                            getSwarm()->setParameterC1( ui_variation_control->getFromValue() );
                        }
                        else if( variable == ui_variation_control->getVariationVariableNames()["c2"] )
                        {
                            getSwarm()->setParameterC2( ui_variation_control->getFromValue() );
                        }
                        else if( variable == ui_variation_control->getVariationVariableNames()["c3"] )
                        {
                            getSwarm()->setParameterC3( ui_variation_control->getFromValue() );
                        }
                        else if( variable == ui_variation_control->getVariationVariableNames()["w"] )
                        {
                            getSwarm()->setParameterW( ui_variation_control->getFromValue() );
                        }
                        else if( variable == ui_variation_control->getVariationVariableNames()["radius"] )
                        {
                            getSwarm()->setNeighbourRadius( ui_variation_control->getFromValue() );
                        }
                        else if( variable == ui_variation_control->getVariationVariableNames()["max velocity"] )
                        {
                            getSwarm()->setMaxVelocity( ui_variation_control->getFromValue() );
                        }

                        average += getSwarm()->optimize( variation_max_iterations );
                        average_fitness += getSwarm()->getBestFitness();
                    }

                    variation_variables_data.push_back( ui_variation_control->getFromValue() );
                    variation_iterations_data.push_back( average / static_cast<double>( average_number ) );
                    variation_fitness_data.push_back( average_fitness / static_cast<double>( average_number ) );

                    variation_variable_curve->setSamples( &*variation_variables_data.begin(), &*variation_iterations_data.begin(), variation_variables_data.size() );
                    variation_fitness_curve->setSamples( &*variation_variables_data.begin(), &*variation_fitness_data.begin(), variation_variables_data.size() );
                    variation_graph->replot();

                    ui_variation_control->setFromValue( ui_variation_control->getFromValue() + ui_variation_control->getStepValue() );
                }
                catch( RuntimeError &err )
                {
                    ui_variation_control->disableTimer();
                    showError( err );
                }
            }

            break;
    }
}

void MainWindow::showError( const RuntimeError &err )
{
    QString line;
    line.setNum( err.getLine() );
    QMessageBox::warning( this, QString( "Error" ), QString::fromStdString( err.getFile() ) + QString( ":" ) + line + QString( "\n" ) + QString::fromStdString( err.getMessage() ) );
}

void MainWindow::particleNumberChanged()
{
    ui_swarm_control->setParticleNumber( swarm.m_swarm.size() );
}

Swarm< Function > *MainWindow::getSwarm()
{
    return &swarm;
}

FunctionViewer *MainWindow::getGLWidget()
{
    return ui_functionviewer;
}

FunctionOptionsWidget *MainWindow::getFunctionOptionsWidget()
{
    return ui_function_options;
}

SwarmControlWidget *MainWindow::getSwarmControlWidget()
{
    return ui_swarm_control;
}

QwtPlot *MainWindow::getVariationPlotWidget()
{
    return variation_graph;
}

VariationControlWidget *MainWindow::getStatisticControlWidget()
{
    return ui_variation_control;
}

QTimer *MainWindow::getTimer()
{
    return timer;
}

ParticleViewWidget *MainWindow::getParticleViewWidget()
{
    return ui_particle_view;
}

GraphWidget *MainWindow::getGraphWidget()
{
    return ui_graph_widget;
}

/**
    Sets the maximum number of allowed iterations in the application mode variations.
*/
void MainWindow::changeVariationMaxIterations()
{
    size_t iterations;
    bool ok;
    iterations = QInputDialog::getInteger( this, "Max Iterations", "max iterations:", variation_max_iterations, 1, 10000000, 1, &ok );

    if( ok )
    {
        variation_max_iterations = iterations;
    }
}

void MainWindow::changeApplicationMode( QAction *action )
{
    if( action->text() == "&3D" )
    {
        setApplicationMode( PSOMode3DView );
    }
    else if( action->text() == "&Variation" )
    {
        setApplicationMode( PSOModeVariation );
    }
    else
    {
        setApplicationMode( PSOMode3DView );
    }
}

/**
    This function enables and disables a couple of user interface
    elements dependent on \a mode.

    \param[in] mode
*/
void MainWindow::setApplicationMode( PSOMode mode )
{
    application_mode = mode;

    ui_swarm_control->changeApplicationMode( mode );
    ui_function_options->changeApplicationMode( mode );

    switch( mode )
    {
        case PSOMode3DView:

            getTimer()->setInterval( ui_swarm_control->getCurrentTimerTimeout() );
            getSwarm()->clear();
            getSwarm()->setDimension( 2 );

            variation_graph->setHidden( true );
            ui_functionviewer->setVisible( true );

            ui_dockwidgets["particle view"]->setEnabled( true );
            ui_dockwidgets["particle view"]->close();

            ui_dockwidgets["graph"]->setEnabled( true );
            ui_dockwidgets["graph"]->close();

            ui_dockwidgets["variation control"]->setEnabled( false );
            ui_dockwidgets["variation control"]->close();


            for( QMap<QString, QAction *>::iterator it = menu_actions_container.begin(); it != menu_actions_container.end(); it++ )
            {
                if( it.key().contains( "options:" ) )
                {
                    it.value()->setEnabled( true );
                }
            }

            menu_actions_container["options:variation max iterations"]->setEnabled( false );

            if( !ui_functionviewer->isFTGLFontLoaded() )
            {
                menu_actions_container["options:show axis"]->setEnabled( false );
            }

            ui_dockmanager->updateDockMenu();
            break;

        case PSOModeVariation:
            getTimer()->stop();

            ui_functionviewer->setHidden( true );
            variation_graph->setVisible( true );

            ui_dockwidgets["particle view"]->setEnabled( false );
            ui_dockwidgets["particle view"]->close();

            ui_dockwidgets["graph"]->setEnabled( false );
            ui_dockwidgets["graph"]->close();

            ui_dockwidgets["variation control"]->show();
            addDockWidget( Qt::LeftDockWidgetArea, ui_dockwidgets["variation control"] );

            for( QMap<QString, QAction *>::iterator it = menu_actions_container.begin(); it != menu_actions_container.end(); it++ )
            {
                if( it.key().contains( "options:" ) )
                {
                    it.value()->setEnabled( false );
                }
            }

            menu_actions_container["options:variation max iterations"]->setEnabled( true );


            ui_dockmanager->updateDockMenu();

            ui_function_options->setFunction();
            break;
    }
}

void MainWindow::clearVariationGraphData()
{
    variation_variables_data.clear();
    variation_iterations_data.clear();
    variation_fitness_data.clear();
}

PSOMode MainWindow::getCurrentApplicationMode() const
{
    return application_mode;
}
/**
    The computation of the next time step is performed in this function. Additionally
    the array to display the particle trace is updated in this function. If particle
    tracing is enabled.
*/
void MainWindow::computeNextStep()
{
    bool trace_particles = ui_functionviewer->isParticleTracingEnabled();
    std::vector<std::vector<Vector<double> > > &trace_particle_container = ui_functionviewer->getTraceParticleContainer();

    if( swarm.getIterationStep() == 0 )
    {
        trace_particle_container.clear();

        if( trace_particles )
        {
            if( trace_particle_container.size() != swarm.m_swarm.size() )
            {
                trace_particle_container.clear();
                trace_particle_container.resize( swarm.m_swarm.size() );
            }

            unsigned int i = 0;
            Swarm<Function>::particle_container::iterator begin = swarm.m_swarm.begin(), end = swarm.m_swarm.end();

            for( Swarm<Function>::particle_container::iterator it( begin ); it != end; it++, i++ )
            {
                trace_particle_container[i].push_back( Vector<double>( ( *it )->getPosition()[0], ( *it )->getPosition()[1], ( *it )->getCurrentValue() + 0.2 ) );
            }
        }
    }

    swarm.computeNextStep();

    if( trace_particles )
    {
        if( trace_particle_container.size() != swarm.m_swarm.size() )
        {
            trace_particle_container.clear();
            trace_particle_container.resize( swarm.m_swarm.size() );
        }

        unsigned int i = 0;
        Swarm<Function>::particle_container::iterator begin = swarm.m_swarm.begin(), end = swarm.m_swarm.end();

        for( Swarm<Function>::particle_container::iterator it( begin ); it != end; it++, i++ )
        {
            trace_particle_container[i].push_back( Vector<double>( ( *it )->getPosition()[0], ( *it )->getPosition()[1], ( *it )->getCurrentValue() + 0.2 ) );
        }
    }
}
