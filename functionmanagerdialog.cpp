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

#include "functionmanagerdialog.h"

FunctionManagerDialog::FunctionManagerDialog( QWidget *parent, Qt::WindowFlags f ) : QDialog( parent, f )
{
    database_filepath = "data/db.txt";

    setWindowTitle( "Function Manager" );
    QHBoxLayout *main_layout = new QHBoxLayout( this );
    setLayout( main_layout );

    ui_function_viewer = new FunctionViewer( this );
    ui_function_viewer->setMinimumSize( QSize( 350, 350 ) );
    setMinimumWidth( 800 );

    QSplitter *splitter = new QSplitter( this );
    main_layout->addWidget( splitter );


    QWidget *widget = new QWidget( this );

    splitter->addWidget( ui_function_viewer );
    splitter->addWidget( widget );

    QVBoxLayout *layout = new QVBoxLayout( widget );

    widget->setLayout( layout );

    QHBoxLayout *layout_boxh = new QHBoxLayout();
    layout->addLayout( layout_boxh );

    QVBoxLayout *layout_boxv = new QVBoxLayout();
    layout_boxh->addLayout( layout_boxv );

    ui_expression = new QLineEdit( "" );
    layout_boxv->addWidget( ui_expression );


    ui_listwidget = new QListWidget;
    connect( ui_listwidget, SIGNAL( clicked( const QModelIndex & ) ), this, SLOT( listWidgetClicked( const QModelIndex & ) ) );
    connect( ui_listwidget, SIGNAL( itemSelectionChanged() ), this, SLOT( listWidgetItemSelectionChanged() ) );

    loadFromFile();

    for( std::vector<db_item>::iterator it( database.begin() ); it != database.end(); it++ )
    {
        QListWidgetItem *item = new QListWidgetItem( ui_listwidget );
        item->setText( QString::fromStdString( it->expression ) );
    }

    layout_boxv->addWidget( ui_listwidget );

    QVBoxLayout *layout_boxv2 = new QVBoxLayout();
    layout_boxh->addLayout( layout_boxv2 );

    ui_add_listing = new QPushButton( "Add" );
    connect( ui_add_listing, SIGNAL( clicked() ), this, SLOT( addExpression() ) );
    layout_boxv2->addWidget( ui_add_listing );
    QPushButton *delete_pushbutton = new QPushButton( "Delete" );
    connect( delete_pushbutton, SIGNAL( clicked() ), this, SLOT( deleteExpression() ) );
    layout_boxv2->addWidget( delete_pushbutton );
    layout_boxv2->addStretch();

    ui_buttonbox = new QDialogButtonBox( Qt::Horizontal );
    QPushButton *load_pushbutton = new QPushButton( "Load" );

    if( database.empty() )
    {
        load_pushbutton->setEnabled( false );
    }

    connect( load_pushbutton, SIGNAL( clicked() ), this, SLOT( load() ) );
    ui_buttonbox->addButton( load_pushbutton, QDialogButtonBox::ActionRole );

    QPushButton *cancel_pushbutton = new QPushButton( "Cancel" );
    ui_buttonbox->addButton( cancel_pushbutton, QDialogButtonBox::RejectRole );

    connect( ui_buttonbox, SIGNAL( accepted() ), this, SLOT( accept() ) );
    connect( ui_buttonbox, SIGNAL( rejected() ), this, SLOT( reject() ) );
    layout->addWidget( ui_buttonbox );
}

FunctionManagerDialog::~FunctionManagerDialog()
{

}

void FunctionManagerDialog::loadFromFile()
{
    database.clear();
    std::ifstream file( database_filepath.c_str() );

    if( file )
    {
        while( file )
        {
            db_item item;
            getline( file, item.expression );

            if( !file ) {break;}

            database.push_back( item );
        }

        file.close();
    }
}

void FunctionManagerDialog::writeToFile()
{
    std::ofstream file( database_filepath.c_str() );

    if( file )
    {
        for( std::vector<db_item>::iterator it( database.begin() ); it != database.end(); it++ )
        {
            file << it->expression << std::endl;
        }

        file.close();
    }
}

/**
    Adds the listing from \a ui_listing to \a ui_listwidget and
    writes all expressions back to the file \a database_filepath.
*/
void FunctionManagerDialog::addExpression()
{
    QString expr = ui_expression->text();

    if( expr.size() != 0 )
    {
        db_item item;
        item.expression = expr.toStdString();
        database.push_back( item );
        ui_listwidget->addItem( expr );
        ui_expression->setText( "" );
        writeToFile();
    }
}

void FunctionManagerDialog::deleteExpression()
{
    QList<QListWidgetItem *> items = ui_listwidget->selectedItems();

    if( !items.empty() )
    {
        QListWidgetItem *item;
        int pos = 0;

        for( int i = 0; i < items.size(); ++i )
        {
            pos = ui_listwidget->row( items[i] );
            item = ui_listwidget->takeItem( pos );

            if( item )
            {
                delete item;
                database.erase( database.begin() + pos );
            }
        }

        writeToFile();
    }
}

void FunctionManagerDialog::setExpression( const QString &expr )
{
    if( expr.size() == 0 )
    {
        ui_add_listing->setEnabled( false );
    }

    ui_expression->setText( expr );
    updatePreview( expr );
}

void FunctionManagerDialog::listWidgetClicked( const QModelIndex &index )
{
    QListWidgetItem *item = ui_listwidget->item( index.row() );
    ui_expression->setText( item->text() );

    updatePreview( item->text() );
}

void FunctionManagerDialog::listWidgetItemSelectionChanged()
{
    QListWidgetItem *item = ui_listwidget->currentItem();
    ui_expression->setText( item->text() );

    updatePreview( item->text() );
}

void FunctionManagerDialog::updatePreview( const QString &expr )
{
    try
    {
        Function f;
        f.setExpression( Function::reduceListingToExpression( expr.toStdString() ) );

        ui_function_viewer->showMiniMap( false );
        ui_function_viewer->setRangeX( -5.0, 5.0, 0.1 );
        ui_function_viewer->setRangeY( -5.0, 5.0, 0.1 );
        ui_function_viewer->setFunction( f );
        ui_function_viewer->setEnabled( true );
        ui_function_viewer->updateGL();

    }
    catch( ... )
    {
        ui_function_viewer->setEnabled( false );
        ui_function_viewer->updateGL();
    }
}

void FunctionManagerDialog::load()
{
    QList<QListWidgetItem *> items = ui_listwidget->selectedItems();

    if( !items.empty() )
    {
        expression = items.first()->text();
        accept();
    }
    else
    {
        QMessageBox::warning( this, QString( "Note" ), "no item selected!" );
    }
}

const QString &FunctionManagerDialog::getExpression()
{
    return expression;
}
