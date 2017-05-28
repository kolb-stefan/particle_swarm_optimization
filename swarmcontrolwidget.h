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

#ifndef SWARMOPTIONWIDGET_H
#define SWARMOPTIONWIDGET_H

#include <QtGui>
#include <QtGui/QWidget>

#include "mainwindow.h"
#include <limits>

class SwarmControlWidget :  public QWidget
{
        Q_OBJECT
    public:
        SwarmControlWidget( MainWindow *mw, QWidget *parent = 0, Qt::WindowFlags f = 0 );
        ~SwarmControlWidget();

        bool isCreationModeRandom();
        unsigned int getCurrentTimerTimeout();

        bool checkFindModeMin();

        void setParticleNumber( unsigned int number );
        unsigned int getParticleNumber();
        bool isAutoVelocityUsed();


        void showUsedIterations( int iterations );
        void showBestPartileFitness( double fitness );
        void showCurrentBestFoundPosition( double x, double y );
        void showCurrentMaxVelocity( double value );

        void changeApplicationMode( PSOMode mode );

        void enableTimer();
        void disableTimer();
    public slots:
        void setFindMinMax();
        void setMaxVelocity();
        void setComputationMode( int index );
        void setSwarmParameter();

        void changeAbortCriterion( bool checked );
        void setSwarmAbortCriterionIterations( int value );
        void changeAutoVelocity( bool checked );
        void setSwarmAutoVelocityMin( double value );

        void changeComputationModeLayout( int index );

        void setTimerTimeout( int timeout );

        void createSwarm();
        void toggleComputation();

    protected:
        QRadioButton        *ui_findmin;
        QRadioButton        *ui_findmax;
        QVector<QVector<QWidget *> >  parameter_widgetlist;
        QComboBox           *ui_methode_switch;
        QSpinBox            *ui_particle_number;
        QPushButton         *ui_create_swarm;
        QComboBox           *ui_swarm_create_methode;
        QDoubleSpinBox      *ui_parameter_c1;
        QDoubleSpinBox      *ui_parameter_c2;
        QDoubleSpinBox      *ui_parameter_c3;
        QDoubleSpinBox      *ui_parameter_w;
        QDoubleSpinBox      *ui_parameter_neighbour_radius;

        QDoubleSpinBox      *ui_max_velocity;
        QCheckBox           *ui_abort_criterion;
        QSpinBox            *ui_abort_criterion_iterations; //holds the number of iterations after that the abort criterion will stop the simulation if global_best->best has not changed

        QCheckBox           *ui_auto_velocity_switch;
        QDoubleSpinBox      *ui_auto_velocity_min;
        QLabel              *ui_auto_velocity_current_status;

        QPushButton         *ui_start_swarm;
        QLabel              *ui_refresh_time_description;
        QSlider             *ui_timeout_slider;
        QLabel              *ui_refresh_time_status;
        QLabel              *ui_iterations_desctiption;
        QLabel              *ui_iterations_status;
        QLabel              *ui_fitness_desctiption;
        QLabel              *ui_fitness_status;
        QLabel              *ui_position_description;
        QLabel              *ui_position_x_status;
        QLabel              *ui_position_y_status;

        MainWindow          *ui_mainwindow;
};

#endif // SWARMOPTIONWIDGET_H
