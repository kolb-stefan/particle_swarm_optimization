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

#include "functionoptionswidget.h"

FunctionOptionsWidget::FunctionOptionsWidget( MainWindow *mw, QWidget *parent, Qt::WindowFlags f ): QWidget( parent, f ), ui_mainwindow( mw )
{
    double min = -1000, max = 1000;

    QBoxLayout *layout = new QBoxLayout( QBoxLayout::TopToBottom, this );
    setLayout( layout );

    QGridLayout *gridlayout_function = new QGridLayout();
    gridlayout_function->addWidget( new QLabel( "Function: " ), 0, 0 );

    ui_current_expression = new QLineEdit( "5*abs(sin(2*sqrt(x1*x1+x2*x2))/sqrt(x1*x1+x2*x2))" );
//     ui_current_expression->setEnabled(false);

    gridlayout_function->addWidget( ui_current_expression, 0, 1 );

    {
        QIcon icon( "/usr/share/icons/oxygen/32x32/categories/applications-education.png" );
        ui_show_function_editor = new QPushButton( icon, "", this );
        connect( ui_show_function_editor, SIGNAL( clicked() ), this, SLOT( showFunctionEditorDialog() ) );
    }
    ui_show_function_editor->setMaximumSize( 25, 25 );
    gridlayout_function->addWidget( ui_show_function_editor, 0, 2 );

    {
        QIcon icon( "/usr/share/icons/oxygen/32x32/apps/knotes.png" );
        ui_show_function_manager = new QPushButton( icon, "", this );
        connect( ui_show_function_manager, SIGNAL( clicked() ), this, SLOT( showFunctionManagerDialog() ) );
    }
    ui_show_function_manager->setMaximumSize( 25, 25 );
    gridlayout_function->addWidget( ui_show_function_manager, 0, 3 );

    layout->addLayout( gridlayout_function );

    {
        ui_container_groupbox = new QGroupBox( "Range" );
        ui_container_groupbox->setFlat( true );
        QGridLayout *gridlayout = new QGridLayout();
        unsigned int row = 0;
        gridlayout->addWidget( new QLabel( "x min: " ), row, 0 );
        gridlayout->addWidget( new QLabel( "x max: " ), row, 1 );
        gridlayout->addWidget( new QLabel( "x step: " ), row, 2 );
        row++;

        ui_low_dim_range_xmin = new QDoubleSpinBox( this );
        ui_low_dim_range_xmin->setMinimumWidth( 10 );
        ui_low_dim_range_xmin->setRange( min, max );
        ui_low_dim_range_xmin->setSingleStep( 0.1 );

        connect( ui_low_dim_range_xmin, SIGNAL( valueChanged( double ) ), this, SLOT( changeFunctionRange() ) );
        gridlayout->addWidget( ui_low_dim_range_xmin, row, 0 );


        ui_low_dim_range_xmax = new QDoubleSpinBox( this );
        ui_low_dim_range_xmax->setMinimumWidth( 10 );
        ui_low_dim_range_xmax->setRange( min, max );
        ui_low_dim_range_xmax->setSingleStep( 0.1 );
        connect( ui_low_dim_range_xmax, SIGNAL( valueChanged( double ) ), this, SLOT( changeFunctionRange() ) );
        gridlayout->addWidget( ui_low_dim_range_xmax, row, 1 );

        ui_low_dim_range_xstep = new QDoubleSpinBox( this );
        ui_low_dim_range_xstep->setMinimumWidth( 10 );
        ui_low_dim_range_xstep->setRange( 0.0001, max );
        ui_low_dim_range_xstep->setSingleStep( 0.01 );
        connect( ui_low_dim_range_xstep, SIGNAL( valueChanged( double ) ), this, SLOT( changeFunctionRange() ) );
        gridlayout->addWidget( ui_low_dim_range_xstep, row, 2 );
        row++;

        gridlayout->addWidget( new QLabel( "y min: " ), row, 0 );
        gridlayout->addWidget( new QLabel( "y max: " ), row, 1 );
        gridlayout->addWidget( new QLabel( "y step: " ), row, 2 );
        row++;

        ui_low_dim_range_ymin = new QDoubleSpinBox( this );
        ui_low_dim_range_ymin->setMinimumWidth( 10 );
        ui_low_dim_range_ymin->setRange( min, max );
        ui_low_dim_range_ymin->setSingleStep( 0.1 );
        connect( ui_low_dim_range_ymin, SIGNAL( valueChanged( double ) ), this, SLOT( changeFunctionRange() ) );
        gridlayout->addWidget( ui_low_dim_range_ymin, row, 0 );


        ui_low_dim_range_ymax = new QDoubleSpinBox( this );
        ui_low_dim_range_ymax->setMinimumWidth( 10 );
        ui_low_dim_range_ymax->setRange( min, max );
        ui_low_dim_range_ymax->setSingleStep( 0.1 );
        connect( ui_low_dim_range_ymax, SIGNAL( valueChanged( double ) ), this, SLOT( changeFunctionRange() ) );
        gridlayout->addWidget( ui_low_dim_range_ymax, row, 1 );


        ui_low_dim_range_ystep = new QDoubleSpinBox( this );
        ui_low_dim_range_ystep->setMinimumWidth( 10 );
        ui_low_dim_range_ystep->setRange( 0.0001, max );
        ui_low_dim_range_ystep->setSingleStep( 0.01 );
        connect( ui_low_dim_range_ystep, SIGNAL( valueChanged( double ) ), this, SLOT( changeFunctionRange() ) );
        gridlayout->addWidget( ui_low_dim_range_ystep, row, 2 );

        ui_low_dim_range_xmin->setValue( -5.0 );
        ui_low_dim_range_xmax->setValue( 5.0 );
        ui_low_dim_range_xstep->setValue( 0.1 );
        ui_low_dim_range_ymin->setValue( -5.0 );
        ui_low_dim_range_ymax->setValue( 5.0 );
        ui_low_dim_range_ystep->setValue( 0.1 );
        ui_container_groupbox->setLayout( gridlayout );
        layout->addWidget( ui_container_groupbox );
    }

    ui_high_dim_range_itemmodel = new QStandardItemModel( 0, 2, this );
    ui_high_dim_range_itemmodel->setHeaderData( 0, Qt::Horizontal, tr( "min" ) );
    ui_high_dim_range_itemmodel->setHeaderData( 1, Qt::Horizontal, tr( "max" ) );
    connect( ui_high_dim_range_itemmodel, SIGNAL( itemChanged( QStandardItem * ) ), this, SLOT( modelitemChanged( QStandardItem * ) ) );

    ui_high_dim_range_view = new QTableView( this );
    ui_high_dim_range_view->setModel( ui_high_dim_range_itemmodel );

    ui_high_dim_range_view->setVisible( false );
    layout->addWidget( ui_high_dim_range_view );

    ui_set_function = new QPushButton( "Set Function" );
    connect( ui_set_function, SIGNAL( clicked() ), this, SLOT( setFunction() ) );
    layout->addWidget( ui_set_function );
    layout->addStretch();
}

FunctionOptionsWidget::~FunctionOptionsWidget()
{

}

void FunctionOptionsWidget::showFunctionEditorDialog()
{
    FunctionEditDialog dialog( this );
    dialog.setExpression( ui_current_expression->text().toStdString() );

    if( dialog.exec() == QDialog::Accepted )
    {
        QString str = QString::fromStdString( dialog.getExpression() );
        ui_current_expression->setText( str );
        setFunction();
    }
}

void FunctionOptionsWidget::showFunctionManagerDialog()
{
    FunctionManagerDialog dialog( this );
    dialog.setExpression( ui_current_expression->text() );

    if( dialog.exec() == QDialog::Accepted )
    {
        ui_current_expression->setText( dialog.getExpression() );
        setFunction();
    }
}

void FunctionOptionsWidget::changeFunctionRange()
{
    FunctionViewer *gl = ui_mainwindow->getGLWidget();
    gl->setRangeX( ui_low_dim_range_xmin->value(), ui_low_dim_range_xmax->value(), ui_low_dim_range_xstep->value() );
    gl->setRangeY( ui_low_dim_range_ymin->value(), ui_low_dim_range_ymax->value(), ui_low_dim_range_ystep->value() );

    gl->regenerateFunction();
    gl->updateGL();
}

/**
    The expression (function) is set corresponding to the application mode. In the case that the
    varriation mode is active it is possible to set function which depend on more more than 2 variables.
*/
void FunctionOptionsWidget::setFunction()
{
    try
    {
        Function function;
//         std::cout << Function::reduceListingToExpression( ui_current_expression->text().toStdString()) << std::endl;
        function.setExpression( Function::reduceListingToExpression( ui_current_expression->text().toStdString() ) );
        ui_mainwindow->getSwarm()->clear();
        ui_mainwindow->getSwarm()->setDimension( function.getNumberOfVariablesInExpression() );
        ui_mainwindow->getSwarm()->setFunction( function );

        if( ui_mainwindow->getCurrentApplicationMode() == PSOMode3DView )
        {
            ui_mainwindow->getTimer()->stop();
            ui_mainwindow->getGLWidget()->setFunction( function );
            ui_mainwindow->getGLWidget()->updateGL();
        }
        else
        {
            fillRangeTable( function.getNumberOfVariablesInExpression() );
        }
    }
    catch( RuntimeError &err )
    {
        ui_mainwindow->showError( err );
    }
}

void FunctionOptionsWidget::getFunctionRange2D( VectorN< double > &min, VectorN< double > &max )
{
    min.resize( 2 );
    max.resize( 2 );

    min[0] = ui_low_dim_range_xmin->value();
    min[1] = ui_low_dim_range_ymin->value();

    max[0] = ui_low_dim_range_xmax->value();
    max[1] = ui_low_dim_range_ymax->value();
}

/**
    Returns the range via the output variables min and max. This function is used in the
    application mode varriation because it support more than two variables in the function.
*/
void FunctionOptionsWidget::getFunctionRange( VectorN< double > &min, VectorN< double > &max )
{
    unsigned int dim = ui_mainwindow->getSwarm()->getDimension();
    min.resize( dim );
    max.resize( dim );

    std::stringstream sstream;

    for( unsigned int i = 0; i < ui_high_dim_range_itemmodel->rowCount(); i++ )
    {

        sstream.str( ui_high_dim_range_itemmodel->item( i, 0 )->text().toStdString() );
        sstream >> min[i];
        sstream.clear();
        sstream.str( ui_high_dim_range_itemmodel->item( i, 1 )->text().toStdString() );
        sstream >> max[i];
        sstream.clear();

        if( max[i] <= min[i] )
        {
            throw RuntimeError( "min value should be less than max value!!" );
        }
    }
}

void FunctionOptionsWidget::changeApplicationMode( PSOMode mode )
{
    if( mode == PSOMode3DView )
    {
        ui_container_groupbox->setVisible( true );
        ui_high_dim_range_view->setVisible( false );

    }
    else if( mode == PSOModeVariation )
    {
        ui_container_groupbox->setVisible( false );
        ui_high_dim_range_view->setVisible( true );
    }
}

void FunctionOptionsWidget::modelitemChanged( QStandardItem *item )
{
    QDoubleValidator validator( this );
    int a;
    validator.setRange( -std::numeric_limits<double>::max(), std::numeric_limits<double>::max() );
    QString text = item->text();

    if( validator.validate( text, a ) != QValidator::Acceptable )
    {
        item->setText( "0.0" );
    }
}

void FunctionOptionsWidget::fillRangeTable( size_t num, double dmin, double dmax )
{
    ui_high_dim_range_itemmodel->removeRows( 0, ui_high_dim_range_itemmodel->rowCount() );
    ui_high_dim_range_itemmodel->insertRows( 0, num );

    QString min, max;

    for( unsigned int i = 0; i < num; i++ )
    {
        if( num <= 2 )
        {
            if( i == 0 )
            {
                min.setNum( ui_low_dim_range_xmin->value() );
                max.setNum( ui_low_dim_range_xmax->value() );
            }
            else if( i == 1 )
            {
                min.setNum( ui_low_dim_range_ymin->value() );
                max.setNum( ui_low_dim_range_ymax->value() );
            }
        }
        else
        {
            min.setNum( dmin );
            max.setNum( dmax );
        }

        ui_high_dim_range_itemmodel->setItem( i, 0, new QStandardItem( min ) );
        ui_high_dim_range_itemmodel->setItem( i, 1, new QStandardItem( max ) );
    }
}

