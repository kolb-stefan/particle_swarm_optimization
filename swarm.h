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

#ifndef SWARM_H_
#define SWARM_H_

#include <stdlib.h>

#include <set>
#include <vector>
#include "particle.h"

template<typename Functor>
class Swarm
{
    public:
        typedef std::vector<Particle *> particle_container;
        enum ComutationMethode {GLOBAL_BEST, GLOBAL_LOCAL_BEST};
        Swarm( size_t dim = 2 ) : dimension( dim ), global_best_particle( NULL )
        {
            compare_function = a_gt_b;
//          m_compare_function = a_lt_b;
            parameter_neighbour_radius = 10;
            parameter_c1 = 2.;
            parameter_c2 = 2.;
            parameter_c3 = 2.;
            parameter_w = 1.;
            computation_methode = GLOBAL_BEST;
            iteration_steps = 0;
//          m_tolerance = 1e-5;
            auto_velocity = false;
            limit_velocity_max = 10.0;
            limit_velocity_min = 0.5;
            global_best_previous = -std::numeric_limits<double>::max();
            global_best_iterations = 0;
            abort_criterion_iterations = 1;

            check_abort_criterion = true;
        }

        virtual ~Swarm()
        {
            clear();
        }

        bool getCheckAbortCriterion()
        {
            return check_abort_criterion;
        }

        void setCheckAbortCriterion( bool c )
        {
            check_abort_criterion = c;
        }

        void setFunction( const Functor &func )
        {
            function = func;
        }

        void addParticke2D( double x1, double x2 )
        {
            Particle *current;
            current = new Particle( dimension );
            current->getPosition()[0] = x1;
            current->getPosition()[1] = x2;
            current->getVelocity()[0] = 0.0;
            current->getVelocity()[1] = 0.0;
            current->initFitness( function );
            m_swarm.push_back( current );
            findGlobalBest();
        }

        void createSwarm( int num, const VectorN<double> &min, const VectorN<double> &max, bool random = false )
        {
            clear();

            if( dimension != min.size() || dimension != max.size() ) {throw RuntimeError( "wrong VectorN dimension" );}

            Particle *current;

            if( dimension == 2 && !random )
            {
                int tmp_num = num;
                int n = ( size_t )sqrt( num ), nx = n;
                int ny = n;

                if( num - nx * ny > 0 )
                {
                    nx++;

                    if( num - nx * ny > 0 )
                    {
                        ny++;
                    }
                }

                double x_step = ( max[0] - min[0] ) / ( double )nx, y_step = ( max[1] - min[1] ) / ( double )ny;

                for( size_t i = 0; i < nx; i++ )
                {
                    for( size_t j = 0; j < ny; j++ )
                    {
                        current = new Particle( dimension );
                        current->getPosition()[0] = min[0] + x_step * i + x_step / 2.;
                        current->getPosition()[1] = min[1] + y_step * j + y_step / 2.;
                        current->getVelocity()[0] = ( min[0] + ( max[0] - min[0] ) * Particle::getRandomNumber() ) * 0.01;
                        current->getVelocity()[1] = ( min[1] + ( max[1] - min[1] ) * Particle::getRandomNumber() ) * 0.01;


                        current->initFitness( function );
                        m_swarm.push_back( current );
                        tmp_num--;

                        if( tmp_num == 0 ) {break;}
                    }

                    if( tmp_num == 0 ) {break;}
                }



            }
            else
            {
                for( size_t i = 0; i < num; i++ )
                {
                    current = new Particle( dimension );


                    for( size_t k = 0; k < dimension; k++ )
                    {
                        current->getPosition()[k] = min[k] + ( max[k] - min[k] ) * Particle::getRandomNumber();
                        current->getVelocity()[k] = ( min[k] + ( max[k] - min[k] ) * Particle::getRandomNumber() ) * 0.01;
                    }

                    current->initFitness( function );
                    m_swarm.push_back( current );
                }
            }

            iteration_steps = 0;
            findGlobalBest();
        }

        size_t getIterationStep()
        {
            return iteration_steps;
        }

        double getBestFitness()
        {
            if( global_best_particle )
            {
                return global_best_particle->getBestValue();
            }
            else
            {
                return 0.;
            }
        }

        Particle *getBestParticle()
        {
            return global_best_particle;
        }

        double getAverageFitness()
        {
            double sum = 0.;

            for( particle_container::iterator it( m_swarm.begin() ); it != m_swarm.end(); it++ )
            {
                sum += ( *it )->getCurrentValue();
            }

            return sum / ( double )m_swarm.size();
        }

        void clear()
        {
            for( particle_container::iterator it( m_swarm.begin() ); it != m_swarm.end(); it++ )
            {
                delete *it;
            }

            m_swarm.clear();
            global_best_particle = NULL;
        }

        void setComputationMethode( ComutationMethode cm )
        {
            computation_methode = cm;
        }

        ComutationMethode getComputationMethode()
        {
            return computation_methode;
        }

        void setDimension( size_t dim )
        {
            dimension = dim;
            clear();
        }


        size_t getDimension()
        {
            return dimension;
        }

        void findGlobalBest()
        {
            if( m_swarm.size() > 0 )
            {
                global_best_particle = *m_swarm.begin();

                for( particle_container::iterator it( m_swarm.begin() ); it != m_swarm.end(); it++ )
                {
                    if( ( *compare_function )( ( *it )->getBestValue(), global_best_particle->getBestValue() ) )
                    {
                        global_best_particle = *it;
                    }
                }
            }
            else
            {
                global_best_particle = NULL;
            }
        }

        void findBestNeighbour( Particle *particle )
        {
            VectorN<double> tmp( dimension );
            particle->setBestNeighbour( NULL );

            for( particle_container::iterator it( m_swarm.begin() ); it != m_swarm.end(); it++ )
            {
                particle->getPosition().sub( ( *it )->getPosition(), tmp );

                if( tmp.length() < parameter_neighbour_radius )
                {
                    if( *it != particle )
                    {
                        if( particle->getBestNeighbour() == NULL )
                        {
                            particle->setBestNeighbour( *it );;
                        }
                        else
                        {
                            if( ( *compare_function )( ( *it )->getCurrentValue(), particle->getBestNeighbour()->getCurrentValue() ) )
                            {
                                particle->setBestNeighbour( *it );;
                            }
                        }
                    }
                }
            }
        }


        bool checkAbortCriterion()
        {
            if( !global_best_particle ) {return false;}

            if( fabs( global_best_particle->getBestValue() - global_best_previous ) < 1e-10 )
            {
                if( global_best_iterations > abort_criterion_iterations )
                {
                    global_best_previous = -std::numeric_limits<double>::max();
                    global_best_iterations = 0;
                    return true;
                }
                else
                {
                    global_best_iterations++;
                    return false;
                }
            }
            else
            {
                global_best_previous = global_best_particle->getBestValue();
                global_best_iterations = 0;
                return false;
            }
        }

        void calculateMaxVelocity()
        {
            if( global_best_iterations != 0 )
            {
                limit_velocity_max -= ( limit_velocity_max - limit_velocity_min ) / 2.;
            }
        }

        void computeNextStep()
        {
            switch( computation_methode )
            {
                case GLOBAL_BEST:
                {
                    VectorN<double> global_best_position( dimension );

                    if( global_best_particle )
                    {
                        global_best_position = global_best_particle->getBestPosition();
                    }
                    else
                    {
                        global_best_position.setAll( 0. );
                    }

                    for( particle_container::iterator it( m_swarm.begin() ); it != m_swarm.end(); it++ )
                    {
                        ( *it )->calcNewGlobal( limit_velocity_max, parameter_c1, parameter_c2, parameter_w, global_best_position );
                    }

                    for( particle_container::iterator it( m_swarm.begin() ); it != m_swarm.end(); it++ )
                    {
                        ( *it )->calculateFitness( function, compare_function );
                    }
                }
                break;

                case GLOBAL_LOCAL_BEST:

                    VectorN<double> global_best_position( dimension );

                    if( global_best_particle )
                    {
                        global_best_position = global_best_particle->getBestPosition();
                    }
                    else
                    {
                        global_best_position.setAll( 0. );
                    }

                    if( !( fabs( parameter_c3 ) < 1e-5 ) )
                    {
                        for( particle_container::iterator it( m_swarm.begin() ); it != m_swarm.end(); it++ )
                        {
                            findBestNeighbour( *it );
                        }
                    }

                    for( particle_container::iterator it( m_swarm.begin() ); it != m_swarm.end(); it++ )
                    {
                        ( *it )->calcNewGlobalAndLocal( limit_velocity_max, parameter_c1, parameter_c2, parameter_c3, parameter_w, global_best_position, compare_function );
                    }

                    for( particle_container::iterator it( m_swarm.begin() ); it != m_swarm.end(); it++ )
                    {
                        ( *it )->calculateFitness( function, compare_function );
                    }

                    break;
            }

            iteration_steps++;
            findGlobalBest();

            if( auto_velocity )
            {
                calculateMaxVelocity();
            }
        }

        size_t optimize( size_t max_iterations = 10000 )
        {
            for( unsigned int i = 0; i < max_iterations; i ++ )
            {
                if( checkAbortCriterion() )
                {
                    return i;
                }

                computeNextStep();
            }

            throw RuntimeError( "to many iterations: no maximum/minimum found" );
        }

        void setCompare( bool comp ) // if comp==true less( a<b) else greater(a>b)
        {
            if( comp )
            {
                compare_function = a_lt_b;
            }
            else
            {
                compare_function = a_gt_b;
            }
        }

        void setAutoVelocity( bool w )
        {
            auto_velocity = w;
        }

        void setMaxVelocity( double vel )
        {
            limit_velocity_max = vel;
        }

        double getMaxVelocity()
        {
            return limit_velocity_max;
        }

        void setMinVelocity( double vel )
        {
            limit_velocity_min = vel;
        }

        void setNeighbourRadius( double dist )
        {
            parameter_neighbour_radius = dist;
        }

        void setParameterC1( double c1 )
        {
            parameter_c1 = c1;
        }

        void setParameterC2( double c2 )
        {
            parameter_c2 = c2;
        }

        void setParameterC3( double c3 )
        {
            parameter_c3 = c3;
        }

        void setParameterW( double w )
        {
            parameter_w = w;
        }

        void setAbortCriterionIterations( size_t i )
        {
            abort_criterion_iterations = i;
        }

        static bool a_lt_b( double a, double b )
        {
            return a < b;
        }

        static bool a_gt_b( double a, double b )
        {
            return a > b;
        }

        particle_container m_swarm;
    protected:
        size_t              dimension;
        double              parameter_neighbour_radius;
        Functor             function;
        Particle            *global_best_particle;
        ComutationMethode   computation_methode;
        bool ( *compare_function )( double, double );
        double              parameter_c1;
        double              parameter_c2;
        double              parameter_c3;
        double              parameter_w;
        size_t              iteration_steps;
        bool                auto_velocity;
        double              limit_velocity_max;
        double              limit_velocity_min;         //the minimum velocity which is set if using auto adjust velocity
        double              global_best_previous;       //fitness of global best after previous iteration
        size_t              global_best_iterations;     //iterations since last change of m_global_best->best
        size_t              abort_criterion_iterations; //breaks if !(m_global_best_iterations < m_abort_criterion_iterations) is true
        bool                check_abort_criterion;
};

#endif
