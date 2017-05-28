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

#include "functionviewer.h"

#include "GL/glu.h"

FunctionViewer::FunctionViewer( QWidget *parent, const QGLWidget *shareWidget, Qt::WindowFlags f )
    : QGLWidget( parent, shareWidget, f ), mini_map_position( 2 ), mini_map_size( 2 ), swarm( NULL )
{
    setDefaultView();

    function_plot_range_x.set( -5.0, 5.0, 0.1 );
    function_plot_range_y.set( -5.0, 5.0, 0.1 );

    setViewModeWireframe( false );
    setViewModePoint( false );

    function_color_min_value[0] = 0.;
    function_color_min_value[1] = 0.;
    function_color_min_value[2] = 1.;

    function_color_max_value[0] = 1.;
    function_color_max_value[1] = 0.;
    function_color_max_value[2] = 0.;

    viewport_background_color[0] = 0.0;
    viewport_background_color[1] = 0.0;
    viewport_background_color[2] = 0.0;

    swarm_point_color[0] = 0.0;
    swarm_point_color[1] = 1.0;
    swarm_point_color[2] = 1.0;
    showSwarm( true );

    mini_map_position[0] = 0.0;
    mini_map_position[1] = 0.0;
    mini_map_size[0] = 1.0 / 4.0;
    mini_map_size[1] = 1.0 / 4.0;
    mini_map_mouse_pressed_translate = false;
    mini_map_mouse_pressed_resize = false;

    showMiniMapGlobalBest( true );

    showMiniMap( true );

    opengl_lists.first = GLLIST_UNDEFINED;

#ifdef USE_FTGL
    QString font_file = "data/arial.ttf";
    ftgl_polygonfont = NULL;

    if( QFile::exists( font_file ) )
    {
        ftgl_polygonfont = new FTPolygonFont( font_file.toAscii() );
    }

    showAxis( true );
    axis_text_scale = 0.3;
    axis_line_length = 0.2;
    axis_number_distance = 0.3;
    axis_label_distance = 0.7;
#endif

    showGlobalBest3D( true );
    showTraceParticles( false );

    particle_point_size = 2.0;

    content_enabled = true;
}

FunctionViewer::~FunctionViewer()
{
#ifdef USE_FTGL

    if( ftgl_polygonfont )
    {
        delete ftgl_polygonfont;
    }

#endif
}

/**
    Function to set the swarm to display. Initial value of
    \a swarm is NULL

    \param[in] s
*/
void FunctionViewer::setSwarm( Swarm< Function > *s )
{
    swarm = s;
}

/**
    Returns a pointer to the currently used swarm instance.

    \returns swarm
*/
Swarm<Function> *FunctionViewer::getSwarm()
{
    return swarm;
}

/**
    In the case that \a swarm is set by \ref setSwarm it is possible to show
    or hide the particles provided by \a swarm dependent on \a w .

    \param[in] w
*/
void FunctionViewer::showSwarm( bool w )
{
    swarm_show = w;
}

/**
    Returns the status if the particles of the \a swarm are displayed or not.

    \returns swarm_show
*/
bool FunctionViewer::isSwarmEnabled()
{
    return swarm_show;
}

/**
    Function to set OpenGL defaults
*/
void FunctionViewer::initializeGL()
{
    glClearColor( viewport_background_color[0], viewport_background_color[1], viewport_background_color[2], 0.0f );

    glClearDepth( 1.0 );
    glDepthFunc( GL_LESS );
    glEnable( GL_DEPTH_TEST );
    glDepthRange( 0, 1 );
    glShadeModel( GL_SMOOTH );

    glViewport( 0, 0, width(), height() );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    gluPerspective( 45.0f, ( GLfloat )width() / ( GLfloat )height(), 0.1f, 500.0f );

    glMatrixMode( GL_MODELVIEW );
}

/**
    Function which is called after every resize of this widget to rescale the content.

    \param[in] width
    \param[in] height
*/
void FunctionViewer::resizeGL( int width, int height )
{
    glViewport( 0, 0, width, height );
    glClearColor( viewport_background_color[0], viewport_background_color[1], viewport_background_color[2], 0.0f );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45.0f, ( GLfloat )width / ( GLfloat )height, 0.1f, 500.0f );
    glMatrixMode( GL_MODELVIEW );
}

/**
    Main OpenGL drawing function. A redraw can be achieved by calling \ref updateGL.
    The whole scene is drawn in this function or from function call here.
*/
void FunctionViewer::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    if( !content_enabled ) {return;}

    genereateOpenglLists();

    if( !function.isEmpty() )
    {
        if( mini_map )
        {
            drawMiniMap();
        }

        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();

        //wireframe
        if( view_mode_wireframe || view_mode_points )
        {
            if( view_mode_wireframe )
            {
                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            }
            else if( view_mode_points )
            {
                glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );
            }
        }
        else
        {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }

        //translation and rotation of the viewport
        glTranslated( viewport_translate[0], viewport_translate[1], viewport_translate[2] );
        glRotated( viewport_rotate[0], 1.0, 0.0, 0.0 );
        glRotated( viewport_rotate[1], 0.0, 1.0, 0.0 );
        glRotated( viewport_rotate[2], 0.0, 0.0, 1.0 );

        glPushMatrix();
        glTranslated( 0, 0, -function_min_value );
        glCallList( opengl_lists.second );

        drawSwarm();
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

#ifdef USE_FTGL

        if( axis_show )
        {
            drawAxis();
        }

#endif

        glPopMatrix();
    }
}

/**
    Draws the minimap. If \a swarm is set by \ref setSwarm as well as enabled
    by \ref showSwarm the particles are shown on the minimap.
*/
void FunctionViewer::drawMiniMap()
{
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    glOrtho( 0, width(), height(), 0., -1, 1 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    double x_min = function_plot_range_x[0], x_max = function_plot_range_x[1], y_min = function_plot_range_y[0], y_max = function_plot_range_y[1];
    double x_length = x_max - x_min, y_length = y_max - y_min;

    double scale = std::min( width() * mini_map_size[0], height() * mini_map_size[1] );
    scale /= std::max( x_length, y_length );


    glScaled( scale, scale, 0. );
    glTranslated( mini_map_position[0] / scale, mini_map_position[1] / scale, 0. );

    {
        glBegin( GL_LINES );
        glColor3f( 1, 1, 1 );
        glVertex2f( 0, 0 );
        glVertex2f( x_length, 0. );

        glVertex2f( x_length, 0. );
        glVertex2f( x_length, y_length );

        glVertex2f( x_length, y_length );
        glVertex2f( 0., y_length );

        glVertex2f( 0., y_length );
        glVertex2f( 0., 0. );
        glEnd();

        glTranslated( -x_min, -y_min, 0. );

        //show best global particle on minimap
        if( mini_map_mark_global_best && ( swarm && swarm_show ) )
        {
            Particle *p = swarm->getBestParticle();

            if( p )
            {
                glBegin( GL_LINES );
                glColor3f( 1, 1, 1 );

                if( p->getBestPosition()[1] >= function_plot_range_y[0] &&  p->getBestPosition()[1] <= function_plot_range_y[1] )
                {
                    glVertex2f( 0. + x_min, p->getBestPosition()[1] );
                    glVertex2f( x_length + x_min, p->getBestPosition()[1] );
                }

                if( p->getBestPosition()[0] >= function_plot_range_x[0] &&  p->getBestPosition()[0] <= function_plot_range_x[1] )
                {
                    glVertex2f( p->getBestPosition()[0], y_length + y_min );
                    glVertex2f( p->getBestPosition()[0], 0. + y_min );
                }

                glEnd();
            }
        }

        drawSwarm( true );
        glCallList( opengl_lists.second + 1 );
    }

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
}

/**
    This function is called every time the mouse is pressed on the widget.

    Needed to move and resize the minimap

    \param[in] event
*/
void FunctionViewer::mousePressEvent( QMouseEvent *event )
{
    mouse_last_position = event->pos();

    if( mini_map )
    {
        double x = event->x(), y = event->y();
        double x_lenght = 0., y_lenght = 0.;
        double zoom_box_lenght = 30;

        calculateMiniMapLengths( x_lenght, y_lenght );

        if( ( ( x > mini_map_position[0] ) && ( x < ( mini_map_position[0] + x_lenght ) ) ) && ( ( y > mini_map_position[1] ) && ( y < ( mini_map_position[1] + y_lenght ) ) ) )
        {
            //add particles to the swarm by clicking on the minimap with the middle mouse button
            if( event->buttons() & Qt::MidButton && ( swarm && swarm_show ) )
            {
                double range_x_length = function_plot_range_x[1] - function_plot_range_x[0], range_y_length = function_plot_range_y[1] - function_plot_range_y[0];
                double scale = std::min( width() * mini_map_size[0], height() * mini_map_size[1] );
                scale /= std::max( range_x_length, range_y_length );

                double xs = function_plot_range_x[0] + ( x - mini_map_position[0] ) / scale;
                double ys = function_plot_range_y[0] + ( y - mini_map_position[1] ) / scale;
                swarm->addParticke2D( xs, ys );
                emit particleNumberChanged();
                updateGL();
            }
            else
            {
                mini_map_mouse_pressed_translate = true;
            }
        }

        if( ( ( x > mini_map_position[0] + x_lenght - zoom_box_lenght / 2. ) && ( x < ( mini_map_position[0] + x_lenght + zoom_box_lenght / 2. ) ) ) && ( ( y > ( mini_map_position[1] + y_lenght - zoom_box_lenght / 2. ) ) && ( y < ( mini_map_position[1] + y_lenght + zoom_box_lenght / 2. ) ) ) )
        {
            mini_map_mouse_pressed_resize = true;
        }
    }
}

/**
    This function is called every time the mouse released.

    Ends the move or resize action for the minimap.

    \param[in] event
*/
void FunctionViewer::mouseReleaseEvent( QMouseEvent *event )
{
    mini_map_mouse_pressed_translate = false;
    mini_map_mouse_pressed_resize = false;
}

/**
    This function is called every time the mouse is moved on the widget.

    It handles the translation and rotation of the viewport as well as
    the moving and resizing of the minimap.

    \param[in] event
*/
void FunctionViewer::mouseMoveEvent( QMouseEvent *event )
{
    double scale_trans = 0.02, scale_rot = 0.2;
    int dx = event->x() - mouse_last_position.x();
    int dy = event->y() - mouse_last_position.y();

    if( event->buttons() & Qt::LeftButton && event->buttons() & Qt::RightButton )
    {
        viewport_rotate[0] += scale_rot * dy;
        viewport_rotate[1] += scale_rot * dx;
    }
    else if( event->buttons() & Qt::LeftButton )
    {
        double x_lenght = 0., y_lenght = 0.;
        calculateMiniMapLengths( x_lenght, y_lenght );

        if( mini_map_mouse_pressed_resize && mini_map )
        {
            double dr = ( dx + dy ) * 0.5;

            if( x_lenght == y_lenght )
            {
                mini_map_size[0] = std::max( ( x_lenght + dr ) / width(), ( y_lenght + dr ) / height() );
                mini_map_size[1] = mini_map_size[0];
            }
            else
            {
                double dr = ( dx + dy ) * 0.5;
                mini_map_size[0] += 0.001 * dr;
                mini_map_size[1] = mini_map_size[0];
            }

            if( mini_map_size[0] > 1 )
            {
                mini_map_size[0] = 1.;
                mini_map_size[1] = mini_map_size[0];
            }
        }
        else
        {
            if( mini_map_mouse_pressed_translate && mini_map )
            {
                mini_map_position[0] += dx;
                mini_map_position[1] += dy;

                if( mini_map_position[0] < 0 ) {mini_map_position[0] = 0;}

                if( ( mini_map_position[0] + x_lenght ) > width() ) {mini_map_position[0] = width() - x_lenght;}

                if( mini_map_position[1] < 0 ) {mini_map_position[1] = 0;}

                if( ( mini_map_position[1] + y_lenght ) > height() ) {mini_map_position[1] = height() - y_lenght;}
            }
            else
            {
                viewport_translate[0] +=  scale_trans * dx;
                viewport_translate[1] -=  scale_trans * dy;
            }
        }
    }
    else if( event->buttons() & Qt::RightButton )
    {
        viewport_rotate[0] += scale_rot * dy;
        viewport_rotate[2] += scale_rot * dx;
    }
    else if( event->buttons() & Qt::MidButton )
    {
        double x = event->x(), y = event->y();
        double x_lenght = 0., y_lenght = 0.;
        calculateMiniMapLengths( x_lenght, y_lenght );

        if( !( ( ( x > mini_map_position[0] ) && ( x < ( mini_map_position[0] + x_lenght ) ) ) && ( ( y > mini_map_position[1] ) && ( y < ( mini_map_position[1] + y_lenght ) ) ) ) )
        {
            viewport_translate[2] += scale_trans * ( dx + dy );
        }
    }

    mouse_last_position = event->pos();

    updateGL();
}

/**
    This function is called for all mouse wheel events of this widget.

    It is used to translate the z-axis ( to zoom in and out).

    \param[in] event
*/
void FunctionViewer::wheelEvent( QWheelEvent *event )
{
    viewport_translate[2] += event->delta() / 50.0;
    updateGL();
}

/**
    Changes the drawing mode of the function to wireframe. The minimap
    as well as the axis are not affected.

    \param[in] w
*/
void FunctionViewer::setViewModeWireframe( bool w )
{
    view_mode_wireframe = w;
    view_mode_points = false;

    updateGL();
}

/**
    Changes the drawing mode of the function to points. The minimap
    as well as the axis are not affected.

    \param[in] w
*/
void FunctionViewer::setViewModePoint( bool w )
{
    view_mode_points = w;
    view_mode_wireframe = false;

    updateGL();
}

bool FunctionViewer::getViewModeWireframe()
{
    return view_mode_wireframe;
}

bool FunctionViewer::getViewModePoint()
{
    return view_mode_points;
}

void FunctionViewer::changeFunctionMinColor()
{
    QColor color = QColorDialog::getColor();

    if( color.isValid() )
    {
        setFunctionMinColor( color );
    }
}

void FunctionViewer::setFunctionMinColor( QColor color )
{
    int r, g, b;
    color.getRgb( &r, &g, &b );
    function_color_min_value[0] = r / 255.;
    function_color_min_value[1] = g / 255.;
    function_color_min_value[2] = b / 255.;
    opengl_lists.first = GLLIST_REBUILD;
    updateGL();
}

void FunctionViewer::changeFunctionMaxColor()
{
    QColor color = QColorDialog::getColor();

    if( color.isValid() )
    {
        setFunctionMaxColor( color );
    }
}

void FunctionViewer::setFunctionMaxColor( QColor color )
{
    int r, g, b;
    color.getRgb( &r, &g, &b );
    function_color_max_value[0] = r / 255.;
    function_color_max_value[1] = g / 255.;
    function_color_max_value[2] = b / 255.;
    opengl_lists.first = GLLIST_REBUILD;
    updateGL();
}

void FunctionViewer::changeBackgroundColor()
{
    QColor col;
    col = QColorDialog::getColor();

    if( col.isValid() )
    {
        setBackgroundColor( col );
    }
}

void FunctionViewer::setBackgroundColor( QColor color )
{
    int r, g, b;
    color.getRgb( &r, &g, &b );
    viewport_background_color[0] = r / 255.;
    viewport_background_color[1] = g / 255.;
    viewport_background_color[2] = b / 255.;
    resizeGL( width(), height() );
    updateGL();
}

void FunctionViewer::setFunction( const Function &func )
{
    function = func;

    if( function.getNumberOfVariablesInExpression() > 2 && !function.isEmpty() )
    {
        throw RuntimeError( "to many variables in equation" );
    }

    regenerateFunction();
}

/**
    Cases a regeneration of the function at the next repaint event
    ergo after or a call to \ref updateGL. The regeneration is done
    in the function \ref paintGL. Additionally the min and max function
    values are reset and also recomputed after a repaint.
*/
void FunctionViewer::regenerateFunction()
{
    function_min_value = std::numeric_limits<double>::max();
    function_max_value = -std::numeric_limits<double>::max();

    if( opengl_lists.first != GLLIST_UNDEFINED )
    {
        opengl_lists.first = GLLIST_REBUILD;
    }
}

/**
    Evaluates \a function at the position \a x and \a y. It is
    a simple wrapper for the operator() of \a function.

    \param[in] x
    \param[in] y
*/
double FunctionViewer::evaluate( double x, double y )
{
    double result;

    if( !function.isEmpty() )
    {
        try
        {
            result = function( x, y );
        }
        catch( RuntimeError &err )
        {
            QString line;
            line.setNum( err.getLine() );
            QMessageBox::warning( this, QString( "Error" ), QString::fromStdString( err.getFile() ) + QString( ":" ) + line + QString( "\n" ) + QString::fromStdString( err.getMessage() ) );
            function.clear();
        }

        if( function_min_value > result ) {function_min_value = result;}

        if( function_max_value < result ) {function_max_value = result;}

        return result;
    }

    return 0.0;
}

/**
    Computes a linear color gradient between the colors \a function_color_min_value
    and \a function_color_max_value where \a value is a number between
    zero and one.

    \param[in] value
    \param[out] r
    \param[out] g
    \param[out] b
*/
void FunctionViewer::calculateColor( double value, double &r, double &g, double &b )
{
    static double color[3];

    for( unsigned int i = 0; i < 3; i++ )
    {
        color[i] = function_color_min_value[i] + ( ( function_color_max_value[i] - function_color_min_value[i] ) * ( ( value - function_min_value ) / ( function_max_value - function_min_value ) ) );
    }

    r = color[0];
    g = color[1];
    b = color[2];
}

/**
    Sets the functions plot range for the x-axis and the spacing for the grid
    on which the function is evaluated.
*/
void FunctionViewer::setRangeX( double min, double max, double step )
{
    if( min < max && step > 0. )
    {
        function_plot_range_x[0] = min;
        function_plot_range_x[1] = max;
        function_plot_range_x[2] = step;

        if( opengl_lists.first == GLLIST_DEFINED )
        {
            opengl_lists.first = GLLIST_REBUILD;
        }
    }
}

/**
    Same as \ref setRangeX but for the y-axis.
*/
void FunctionViewer::setRangeY( double min, double max, double step )
{
    if( min < max && step > 0. )
    {
        function_plot_range_y[0] = min;
        function_plot_range_y[1] = max;
        function_plot_range_y[2] = step;

        if( opengl_lists.first == GLLIST_DEFINED )
        {
            opengl_lists.first = GLLIST_REBUILD;
        }
    }
}

void FunctionViewer::showMiniMap( bool w )
{
    mini_map = w;
    updateGL();
}

bool FunctionViewer::isMiniMapEnabled()
{
    return mini_map;
}

/**
    Draws the swarm as well as a marker for the global best particle.
    If \a draw_on_minimap is true the particles are drawn as point on
    the minimap.

    \param[in] draw_on_minimap
*/
void FunctionViewer::drawSwarm( bool draw_on_minimap )
{
    if( swarm && swarm_show )
    {
        glPointSize( particle_point_size );

        glColor3f( 0.0, 1.0, 0.0 );

        if( !draw_on_minimap )
        {
            if( mark_global_best_3d )
            {
                if( swarm->getBestParticle() )
                {
                    static GLUquadricObj *quadratic = gluNewQuadric();

                    glPushMatrix();
                    glTranslated( swarm->getBestParticle()->getBestPosition()[0], swarm->getBestParticle()->getBestPosition()[1], swarm->getBestParticle()->getBestValue() + 1.0 );
                    gluCylinder( quadratic, 0.0, 0.15, 0.4, 32, 32 );
                    glTranslated( 0.0, 0.0, 0.4 );
                    gluCylinder( quadratic, 0.075, 0.075, 0.7, 32, 32 );
                    glPopMatrix();
                }
            }

            glColor3f( swarm_point_color[0], swarm_point_color[1], swarm_point_color[2] );

            if( trace_particles )
            {
                for( std::vector<std::vector<Vector<double> > >::iterator it1( trace_particles_container.begin() ); it1 != trace_particles_container.end(); it1++ )
                {
                    glBegin( GL_LINE_STRIP );

                    for( std::vector<Vector<double> >::iterator it2( it1->begin() ); it2 != it1->end() - 1; it2++ )
                    {
                        glVertex3f( ( *it2 )[0], ( *it2 )[1], ( *it2 )[2] );
                    }

                    glEnd();
                }
            }
        }

        glBegin( GL_POINTS );

        Swarm<Function>::particle_container::iterator begin = swarm->m_swarm.begin(), end = swarm->m_swarm.end();

        for( Swarm<Function>::particle_container::iterator it( begin ); it != end; it++ )
        {
            if( draw_on_minimap )
            {
                glColor3f( swarm_point_color[0], swarm_point_color[1], swarm_point_color[2] );

                if( ( *it )->getPosition()[0] >= function_plot_range_x[0] && ( *it )->getPosition()[0] <= function_plot_range_x[1] && ( *it )->getPosition()[1] >= function_plot_range_y[0] && ( *it )->getPosition()[1] <= function_plot_range_y[1] )
                {
                    glVertex2f( ( *it )->getPosition()[0], ( *it )->getPosition()[1] );
                }
            }
            else
            {
                glVertex3f( ( *it )->getPosition()[0], ( *it )->getPosition()[1], ( *it )->getCurrentValue() + 0.2 );
            }
        }

        glEnd();
        glPointSize( 1.0 );
    }
}

/**
    Generates OpenGL lists for the 3d function as well as for the minimap.
*/
void FunctionViewer::genereateOpenglLists()
{
    if( !function.isEmpty() && ( opengl_lists.first == GLLIST_REBUILD || opengl_lists.first == GLLIST_UNDEFINED ) )
    {
        if( opengl_lists.first == GLLIST_REBUILD )
        {
            glDeleteLists( opengl_lists.second, 2 );
        }

        opengl_lists.second = glGenLists( 2 );

        /*
            For finding the min and max function value see evaluate(x,y) for more information. The min and max value
            is needed to scale the actual function value to a number between zero and one. This is neede to compute
            the right color for a specific point.
        */
        for( double x = function_plot_range_x[0]; x < function_plot_range_x[1]; x += function_plot_range_x[2] )
        {
            for( double y = function_plot_range_y[0]; y < function_plot_range_y[1]; y += function_plot_range_y[2] )
            {
                evaluate( x, y );
            }
        }

        //generates list for the function
        glNewList( opengl_lists.second, GL_COMPILE );
        {
            double r, g, b, functionvalue;
            double x_tmp, y_tmp;

            for( double x = function_plot_range_x[0]; x < ( function_plot_range_x[1] - function_plot_range_x[2] ); x += function_plot_range_x[2] )
            {
                for( double y = function_plot_range_y[0]; y < ( function_plot_range_y[1] - function_plot_range_y[2] ); y += function_plot_range_y[2] )
                {
                    glBegin( GL_QUADS );
                    x_tmp = x;
                    y_tmp = y;
                    functionvalue = evaluate( x_tmp, y_tmp );
                    calculateColor( functionvalue, r, g, b );
                    glColor3f( r, g, b );
                    glVertex3f( x_tmp, y_tmp, functionvalue );

                    x_tmp = x + function_plot_range_x[2];
                    y_tmp = y;
                    functionvalue = evaluate( x_tmp, y_tmp );
                    calculateColor( functionvalue, r, g, b );
                    glColor3f( r, g, b );
                    glVertex3f( x_tmp, y_tmp, functionvalue );

                    x_tmp = x + function_plot_range_x[2];
                    y_tmp = y + function_plot_range_y[2];
                    functionvalue = evaluate( x_tmp, y_tmp );
                    calculateColor( functionvalue, r, g, b );
                    glColor3f( r, g, b );
                    glVertex3f( x_tmp, y_tmp, functionvalue );

                    x_tmp = x;
                    y_tmp = y + function_plot_range_y[2];
                    functionvalue = evaluate( x_tmp, y_tmp );
                    calculateColor( functionvalue, r, g, b );
                    glColor3f( r, g, b );
                    glVertex3f( x_tmp, y_tmp, functionvalue );

                    glEnd();
                }
            }
        }
        glEndList();

        //generates list for the minimap
        glNewList( opengl_lists.second + 1, GL_COMPILE );
        {
            double r, g, b, functionvalue;
            double x_tmp, y_tmp;

            for( double x = function_plot_range_x[0]; x < ( function_plot_range_x[1] - function_plot_range_x[2] ); x += function_plot_range_x[2] )
            {
                for( double y = function_plot_range_y[0]; y < ( function_plot_range_y[1] - function_plot_range_y[2] ); y += function_plot_range_y[2] )
                {
                    glBegin( GL_QUADS );
                    x_tmp = x;
                    y_tmp = y;
                    functionvalue = evaluate( x_tmp, y_tmp );
                    calculateColor( functionvalue, r, g, b );
                    glColor3f( r, g, b );
                    glVertex2f( x_tmp, y_tmp );

                    x_tmp = x + function_plot_range_x[2];
                    y_tmp = y;
                    functionvalue = evaluate( x_tmp, y_tmp );
                    calculateColor( functionvalue, r, g, b );
                    glColor3f( r, g, b );
                    glVertex2f( x_tmp, y_tmp );

                    x_tmp = x + function_plot_range_x[2];
                    y_tmp = y + function_plot_range_y[2];
                    functionvalue = evaluate( x_tmp, y_tmp );
                    calculateColor( functionvalue, r, g, b );
                    glColor3f( r, g, b );
                    glVertex2f( x_tmp, y_tmp );

                    x_tmp = x;
                    y_tmp = y + function_plot_range_y[2];
                    functionvalue = evaluate( x_tmp, y_tmp );
                    calculateColor( functionvalue, r, g, b );
                    glColor3f( r, g, b );
                    glVertex2f( x_tmp, y_tmp );

                    glEnd();
                }
            }
        }
        glEndList();
        opengl_lists.first = GLLIST_DEFINED;
    }
}

void FunctionViewer::calculateMiniMapLengths( double &x_length_, double &y_length_ )
{
    double range_x_length = function_plot_range_x[1] - function_plot_range_x[0], range_y_length = function_plot_range_y[1] - function_plot_range_y[0];
    double scale = std::min( width() * mini_map_size[0], height() * mini_map_size[1] );
    scale /= std::max( range_x_length, range_y_length );

    x_length_ = range_x_length * scale;
    y_length_ = range_y_length * scale;
}

void FunctionViewer::setDefaultView()
{
    viewport_rotate.set( -45.0, 0.0, 0.0 );
    viewport_translate.set( 0.0, 0.0, -15.0 );
    updateGL();
}

#ifdef USE_FTGL
void FunctionViewer::drawAxis()
{
    drawXYZAxis( 'x' );
    drawXYZAxis( 'y' );
    drawXYZAxis( 'z' );
}

void FunctionViewer::drawXYZAxis( char axis )
{
    if( !ftgl_polygonfont )
    {
        return;
    }

    ftgl_polygonfont->FaceSize( 1 );
    std::ostringstream oss;
    double start = 0., stop = 0.;
    double string_width = 0.;
    double current_pos = 0.;
    double distance = 0.;

    bool cord = false;
    double cord_move;
    double cord_rotate;
    double cord_sign;

    size_t n = 0;
    glColor3f( 1., 1., 1. );

    if( axis == 'x' )
    {
        cord = ( static_cast<size_t>( fabs( viewport_rotate[2] ) ) % 360 ) < 90 || ( static_cast<size_t>( fabs( viewport_rotate[2] ) ) % 360 ) > 270;

        if( cord )
        {
            cord_sign = 1.0;
            cord_move = 0.0;
            cord_rotate = 0.0;
        }
        else
        {
            cord_sign = -1.0;
            cord_move = function_plot_range_y[1] - function_plot_range_y[0];
            cord_rotate = 180;
        }

        glBegin( GL_LINES );
        glVertex3f( function_plot_range_x[0] , function_plot_range_y[0] + cord_move  , function_min_value );
        glVertex3f( function_plot_range_x[1] , function_plot_range_y[0] + cord_move , function_min_value );
        glEnd();


        start = ceil( function_plot_range_x[0] );
        stop = floor( function_plot_range_x[1] );
        n = static_cast<size_t>( floor( function_plot_range_x[1] - function_plot_range_x[0] ) );
    }
    else if( axis == 'y' )
    {
        cord = ( static_cast<size_t>( fabs( viewport_rotate[2] - 90 ) ) % 360 ) < 90 || ( static_cast<size_t>( fabs( viewport_rotate[2] - 90 ) ) % 360 ) > 270;

        if( cord )
        {
            cord_sign = -1.0;
            cord_move = 0.0;
            cord_rotate = 0.0;
        }
        else
        {
            cord_sign = +1.0;
            cord_move = function_plot_range_x[1] - function_plot_range_x[0];
            cord_rotate = 180;
        }

        glBegin( GL_LINES );
        glVertex3f( function_plot_range_x[0] + cord_move , function_plot_range_y[0] , function_min_value );
        glVertex3f( function_plot_range_x[0] + cord_move , function_plot_range_y[1] , function_min_value );
        glEnd();


        start = ceil( function_plot_range_y[0] );
        stop = floor( function_plot_range_y[1] );
        n = static_cast<size_t>( floor( function_plot_range_y[1] - function_plot_range_y[0] ) );
    }
    else if( axis == 'z' )
    {
        int angle = static_cast<int>( ceil( viewport_rotate[2] ) ) % 360;
        double gl_angle = 0;
        double length_x =  function_plot_range_x[1] - function_plot_range_x[0], length_y = function_plot_range_y[1] - function_plot_range_y[0];

        glPushMatrix();

        if( ( angle >= -45 &&  angle <= 45 ) || ( angle >= 315 && angle <= 360 ) || ( angle >= -360 &&  angle <= -315 ) )
        {
            glTranslated( function_plot_range_x[0], function_plot_range_y[0], 0.0 );
            gl_angle = 0;
        }
        else if( ( angle >= 45 && angle <= 135 ) || ( angle >= -315 &&  angle <= -225 ) )
        {
            glTranslated( function_plot_range_x[0], function_plot_range_y[0] + length_y, 0.0 );
            gl_angle = -90;
        }
        else if( ( angle >= 135 && angle <= 225 ) || ( angle >= -225 &&  angle <= -135 ) )
        {
            glTranslated( function_plot_range_x[0] + length_x, function_plot_range_y[0] + length_y, 0.0 );
            gl_angle = -180;
        }
        else if( ( angle >= 225 && angle <= 315 ) || ( angle >= -135 &&  angle <= -45 ) )
        {
            glTranslated( function_plot_range_x[0] + length_x, function_plot_range_y[0], 0.0 );
            gl_angle = -270;
        }

        glRotated( gl_angle, 0., 0., 1. );

        start = ceil( function_min_value );
        stop = floor( function_max_value );

        glBegin( GL_LINES );
        glVertex3f( 0.0, 0.0 , function_max_value );
        glVertex3f( 0.0, 0.0 , function_min_value );
        glEnd();

        n = static_cast<size_t>( floor( function_max_value - function_min_value ) );

        if( n > 1000 )
        {
            n = 1000;
        }
    }

    for( size_t i = 0; i < n + 2; i++ )
    {
        string_width = 0.;


        if( i == n + 1 )
        {
            distance = axis_label_distance;

            if( axis == 'x' )
            {
                current_pos = function_plot_range_x[0] + ( function_plot_range_x[1] - function_plot_range_x[0] ) / 2.0;
                oss << "x-Axis";
            }
            else if( axis == 'y' )
            {
                current_pos = function_plot_range_y[0] + ( function_plot_range_y[1] - function_plot_range_y[0] ) / 2.0;
                oss << "y-Axis";
            }
            else if( axis == 'z' )
            {
                current_pos = function_min_value + ( function_max_value - function_min_value ) / 2.0;
                oss << "z-Axis";
            }
        }
        else
        {
            distance = axis_number_distance;
            current_pos = start + 1.0 * i;
            oss << current_pos;
        }

        string_width = ftgl_polygonfont->Advance( oss.str().c_str() );

        glPushMatrix();


        if( i != n + 1 )
        {
            glBegin( GL_LINES );

            if( axis == 'x' )
            {
                glVertex3f( current_pos , function_plot_range_y[0] + cord_move  , function_min_value + axis_line_length / 2.0 );
                glVertex3f( current_pos , function_plot_range_y[0] + cord_move , function_min_value - axis_line_length / 2.0 );
            }
            else if( axis == 'y' )
            {
                glVertex3f( function_plot_range_x[0] + cord_move, current_pos , function_min_value + axis_line_length / 2.0 );
                glVertex3f( function_plot_range_x[0] + cord_move, current_pos , function_min_value - axis_line_length / 2.0 );
            }
            else if( axis == 'z' )
            {
                glVertex3f( axis_line_length / 2.0, 0., current_pos );
                glVertex3f( 0.0 - axis_line_length / 2.0, 0., current_pos );
            }

            glEnd();
        }

        glScaled( axis_text_scale, axis_text_scale, axis_text_scale );

        if( axis == 'x' )
        {
            glTranslated( current_pos / axis_text_scale - ( string_width / 2.0 ) * cord_sign, ( function_plot_range_y[0] + cord_move ) / axis_text_scale , function_min_value / axis_text_scale - ftgl_polygonfont->LineHeight() * axis_text_scale - distance / axis_text_scale );
            glRotated( cord_rotate, 0., 0., 1. );
            glRotated( 90., 1., 0., 0. );
        }
        else if( axis == 'y' )
        {
            glTranslated( ( function_plot_range_x[0] + cord_move ) / axis_text_scale, current_pos / axis_text_scale - ( string_width / 2.0 ) * cord_sign , function_min_value / axis_text_scale - ftgl_polygonfont->LineHeight() * axis_text_scale - distance / axis_text_scale );
            glRotated( cord_rotate, 0., 0., 1. );
            glRotated( 90., 1., 0., 0. );
            glRotated( -90., 0., 1., 0. );
        }

        if( axis == 'z' )
        {
            if( i == n + 1 )
            {
                glTranslated( -distance / axis_text_scale - ( ftgl_polygonfont->LineHeight() / 2.0 ) * axis_text_scale, 0.0, current_pos / axis_text_scale  - ( string_width / 2.0 ) );
                glRotated( 90., 1., 0., 0. );
                glRotated( 90., 0., 0., 1. );
            }
            else
            {
                glTranslated( -distance / axis_text_scale - ( string_width / 2.0 ), 0.0, current_pos / axis_text_scale - ( ftgl_polygonfont->LineHeight() / 2.0 ) * axis_text_scale );
                glRotated( 90., 1., 0., 0. );
            }
        }

        ftgl_polygonfont->Render( oss.str().c_str() );
        oss.str( "" );
        oss.clear();
        glPopMatrix();
    }

    if( axis == 'z' )
    {
        glPopMatrix();
    }
}

void FunctionViewer::showAxis( bool b )
{
    axis_show = b;
    updateGL();
}

bool FunctionViewer::isFTGLFontLoaded()
{
    if( ftgl_polygonfont )
    {
        return true;
    }
    else
    {
        return false;
    }
}

#endif

void FunctionViewer::showMiniMapGlobalBest( bool w )
{
    mini_map_mark_global_best = w;
    updateGL();
}

void FunctionViewer::showTraceParticles( bool w )
{
    trace_particles = w;
    trace_particles_container.clear();
    updateGL();
}

bool FunctionViewer::isParticleTracingEnabled()
{
    return trace_particles;
}

void FunctionViewer::changePointSize()
{
    double point_size;
    bool ok;

    point_size = QInputDialog::getDouble( this, "PointSize", "change point size:", getPointSize(), 0.1, 100, 1, &ok );

    if( ok )
    {
        setPointSize( point_size );
    }
}

void FunctionViewer::setPointSize( double size )
{
    particle_point_size = size;
    updateGL();
}

double FunctionViewer::getPointSize()
{
    return particle_point_size;
}

void FunctionViewer::changePointColor()
{
    QColor col;
    col = QColorDialog::getColor();

    if( col.isValid() )
    {
        setPointColor( col );
    }
}

void FunctionViewer::setPointColor( QColor color )
{
    int r, g, b;
    color.getRgb( &r, &g, &b );
    swarm_point_color[0] = r / 255.;
    swarm_point_color[1] = g / 255.;
    swarm_point_color[2] = b / 255.;
    updateGL();
}

void FunctionViewer::showGlobalBest3D( bool w )
{
    mark_global_best_3d = w;
    updateGL();
}

std::vector< std::vector< Vector< double > > > &FunctionViewer::getTraceParticleContainer()
{
    return trace_particles_container;
}

void FunctionViewer::setEnabled( bool w )
{
    content_enabled = w;
    updateGL();
}
