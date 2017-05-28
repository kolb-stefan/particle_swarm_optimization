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

#include "functioneditdialog.h"

FunctionEditDialogWorker::FunctionEditDialogWorker( QObject *parent ): QThread( parent )
{

}

FunctionEditDialogWorker::~FunctionEditDialogWorker()
{

}

void FunctionEditDialogWorker::setExpression( QString expr )
{
    expression = expr;
}

void FunctionEditDialogWorker::run()
{
    try
    {
        Function f;
        f.setExpression( Function::reduceListingToExpression( expression.toStdString() ) );
        emit equationChecked(expression);
    }
    catch( ... )
    {
        emit equationChecked(QString());
    }
}


FunctionEditDialog::FunctionEditDialog( QWidget *parent, Qt::WindowFlags f ) : QDialog( parent, f )
{
    setWindowTitle( "Function Editor" );

    if( parent != NULL )
    {
        QSize s = parent->size();
        s.setHeight( s.height() * 0.7 );
        s.setWidth( s.width() * 0.7 );
        resize( s );
    }

    QSplitter *splitter = new QSplitter( this );
    QHBoxLayout *layout = new QHBoxLayout( this );
    layout->addWidget( splitter );
    setLayout( layout );

    ui_function_viewer = new FunctionViewer( this );
    ui_function_viewer->setMinimumSize( QSize( 350, 350 ) );
    setMinimumWidth( 900 );

    QWidget *widget = new QWidget( this );

    splitter->addWidget( ui_function_viewer );
    splitter->addWidget( widget );

    QVBoxLayout *bl = new QVBoxLayout( widget );
    widget->setLayout( bl );
    ui_expression_edit = new QTextEdit( widget );
    ui_expression_edit->setAcceptRichText( false );
    connect( ui_expression_edit, SIGNAL( textChanged() ), this, SLOT( expressionChanged() ) );
    bl->addWidget( ui_expression_edit );

    ui_buttonbox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
    connect( ui_buttonbox, SIGNAL( accepted() ), this, SLOT( accept() ) );
    connect( ui_buttonbox, SIGNAL( rejected() ), this, SLOT( reject() ) );
    bl->addWidget( ui_buttonbox );

    timer.setInterval( 300 );
    connect( &timer, SIGNAL( timeout() ), this, SLOT( timerTimeOut() ) );
    
    connect(&worker, SIGNAL(equationChecked(QString)), this, SLOT(updatePreview(QString)));
}

FunctionEditDialog::~FunctionEditDialog()
{

}

void FunctionEditDialog::setExpression( const std::string &text )
{
    std::string expression;

    for( std::string::const_iterator it( text.begin() ); it != text.end(); it++ )
    {
        if( *it == ';' && text.end() != it + 1 )
        {
            expression.push_back( *it );
            expression.push_back( '\n' );
        }
        else
        {
            expression.push_back( *it );
        }
    }

    ui_expression_edit->setPlainText( QString::fromStdString( expression ) );
    updatePreview(ui_expression_edit->toPlainText());
}

std::string FunctionEditDialog::getExpression( void )
{
    std::string expression, text;
    text = ui_expression_edit->toPlainText().toStdString();

    for( std::string::const_iterator it( text.begin() ); it != text.end(); it++ )
    {
        if( *it != '\n' )
        {
            expression.push_back( *it );
        }
    }

    return expression;
}

void FunctionEditDialog::updatePreview( const QString &expr )
{
    try
    {
        if(expr.isEmpty())
        {
            throw RuntimeError("");
        }
        
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
//         std::cout << "Exception cought!" << std::endl;
        ui_function_viewer->setEnabled( false );
        ui_function_viewer->updateGL();
    }
}

void FunctionEditDialog::timerTimeOut()
{
    if(!worker.isRunning())
    {
        worker.setExpression(ui_expression_edit->toPlainText());
        worker.start();
    }
    
    timer.stop();
}

void FunctionEditDialog::expressionChanged()
{
    timer.stop();

    timer.start();
}
