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

#include "particleviewwidget.h"
#include "swarmcontrolwidget.h"

ParticleViewWidget::ParticleViewWidget( MainWindow *mw , QWidget *parent, Qt::WindowFlags f ) : QWidget( parent, f ), ui_mainwindow( mw )
{
    QBoxLayout *layout = new QBoxLayout( QBoxLayout::TopToBottom, this );
    setLayout( layout );

    particle_model = new QStandardItemModel( 0, 3, this );
    particle_model->setHeaderData( 0, Qt::Horizontal, tr( "Fitness" ) );
    particle_model->setHeaderData( 1, Qt::Horizontal, tr( "Position" ) );
    particle_model->setHeaderData( 2, Qt::Horizontal, tr( "Velocity" ) );


    QTableView *particle_view_tableview = new QTableView( this );
    particle_view_tableview->setModel( particle_model );
    layout->addWidget( particle_view_tableview );
}

ParticleViewWidget::~ParticleViewWidget()
{

}

bool compare_a_gt_b( const std::pair<double, Particle *> &a, const std::pair<double, Particle *> &b )
{
    return a.first > b.first;
}

bool compare_a_lt_b( const std::pair<double, Particle *> &a, const std::pair<double, Particle *> &b )
{
    return a.first < b.first;
}

/**
    Adds the fitness, position and velocity of each particle in the swarm
    to the table and sorts the particles after its fitness.
*/
void ParticleViewWidget::updateView()
{
    Swarm<Function> *swarm = ui_mainwindow->getSwarm();

    if( static_cast<size_t>( particle_model->rowCount() ) != swarm->m_swarm.size() )
    {
        particle_model->removeRows( 0, particle_model->rowCount() );
        particle_model->insertRows( 0, swarm->m_swarm.size() );
    }

    std::vector<std::pair<double, Particle *> > sort_container( swarm->m_swarm.size() );
    Swarm<Function>::particle_container::iterator begin = swarm->m_swarm.begin(), end = swarm->m_swarm.end();
    {
        size_t i = 0;

        for( Swarm<Function>::particle_container::iterator it( begin ); it != end; it++, i++ )
        {
            sort_container[i] = std::make_pair( ( *it )->getCurrentValue(), *it );
        }
    }

    if( ui_mainwindow->getSwarmControlWidget()->checkFindModeMin() )
    {
        sort( sort_container.begin(), sort_container.end(), compare_a_lt_b );
    }
    else
    {
        sort( sort_container.begin(), sort_container.end(), compare_a_gt_b );
    }

    {
        QString num;
        unsigned int row = 0;

        for( std::vector<std::pair<double, Particle *> >::iterator it( sort_container.begin() ); it != sort_container.end(); it++ )
        {
            num.setNum( it->second->getCurrentValue() );
            particle_model->setItem( row, 0, new QStandardItem( num ) );
            particle_model->setItem( row, 1, new QStandardItem( QString::fromStdString( it->second->getPosition().toString( 3 ) ) ) );
            particle_model->setItem( row, 2, new QStandardItem( QString::fromStdString( it->second->getVelocity().toString( 3 ) ) ) );
            row++;
        }
    }
}
