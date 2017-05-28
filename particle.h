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

#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <stdlib.h>

#include <limits>

#include "vectorn.h"

class Particle
{
    public:
        Particle( size_t dim );
        virtual ~Particle();

        template<typename Functor>
        void calculateFitness( Functor &func, bool ( *compare )( double, double ) );
        template<typename Functor>
        void initFitness( Functor &func );

        void calcNewGlobal( double max_velocity, double c1, double c2, double w, VectorN<double> &global_best );

        void calcNewGlobalAndLocal( double max_velocity, double c1, double c2, double c3, double w, VectorN<double> &global_best, bool ( *compare )( double, double ) );

        static double getRandomNumber();

        VectorN<double> &getPosition();
        VectorN<double> &getVelocity();
        VectorN<double> &getBestPosition();

        double getCurrentValue();
        double getBestValue();

        Particle *getBestNeighbour();
        void setBestNeighbour( Particle *bn );

    protected:
        VectorN<double>     position;
        VectorN<double>     velocity;
        double              current_value;      //current function value: (<->present)

        double              best_value;         //best ever found function value: (<->pbest)
        VectorN<double>     best_position;      //best ever found position: (<->pbest)

        Particle            *best_neighbour;    //a pointer to the neighbour with the best value
};

template<typename Functor>
void Particle::calculateFitness( Functor &func, bool ( *compare )( double, double ) )
{
    current_value = func( position );

    if( ( *compare )( current_value, best_value ) )
    {
        best_value = current_value;
        best_position = position;
    }
}

template<typename Functor>
void Particle::initFitness( Functor &func )
{
    current_value = func( position );
    best_value = current_value;
    best_position = position;
}

#endif
