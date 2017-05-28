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

#include "particle.h"

Particle::Particle( size_t dim ) : position( dim ), velocity( dim ), best_position( dim )
{
    current_value = 0.;
    best_value = 0.0;//numeric_limits<double>::min();
    best_position = position;
    best_neighbour = NULL;
}

Particle::~Particle()
{

}

void Particle::calcNewGlobal( double max_velocity, double c1, double c2, double w, VectorN<double> &global_best )
{
    velocity = velocity * w + ( best_position - position ) * c1 * getRandomNumber() + ( global_best - position ) * c2 * getRandomNumber();

    if( velocity.length() >= max_velocity )
    {
        velocity.normalize();
        velocity = velocity * max_velocity;
    }

    position = position + velocity;
}

void Particle::calcNewGlobalAndLocal( double max_velocity, double c1, double c2, double c3, double w, VectorN<double> &global_best, bool ( *compare )( double, double ) )
{
    if( best_neighbour )
    {
        velocity = velocity * w + ( best_position - position ) * c1 * getRandomNumber() + ( global_best - position ) * c2 * getRandomNumber() + ( best_neighbour->position - position ) * c3 * getRandomNumber();
    }
    else
    {
        velocity = velocity * w + ( best_position - position ) * c1 * getRandomNumber() + ( global_best - position ) * c2 * getRandomNumber();

    }

    if( velocity.length() >= max_velocity )
    {
        velocity.normalize();
        velocity = velocity * max_velocity;
    }

    position = position + velocity;
}

VectorN< double > &Particle::getPosition()
{
    return position;
}

VectorN< double > &Particle::getVelocity()
{
    return velocity;
}

VectorN< double > &Particle::getBestPosition()
{
    return best_position;
}

double Particle::getCurrentValue()
{
    return current_value;
}

double Particle::getBestValue()
{
    return best_value;
}

Particle *Particle::getBestNeighbour()
{
    return best_neighbour;
}

void Particle::setBestNeighbour( Particle *bn )
{
    best_neighbour = bn;
}

double Particle::getRandomNumber()
{
#ifdef win32
    return ( ::rand() % RAND_MAX ) / ( ( double )( RAND_MAX - 1 ) );
#endif

#ifdef __gnu_linux__
    return ( ::random() % RAND_MAX ) / ( ( double )( RAND_MAX - 1 ) );
#endif

}
