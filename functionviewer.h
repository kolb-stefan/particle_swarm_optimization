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

#ifndef GLWIDGET_H
#define GLWIDGET_H

#ifdef USE_FTGL
#include <FTGL/ftgl.h>
#endif

#include <Qt>
#include <QtGui>
#include <QtOpenGL/QGLWidget>

#include <cmath>
#include <string>
#include <vector>

#include "vectorn.h"
#include "vector.h"
#include "function.h"
#include "swarm.h"

class FunctionViewer : public QGLWidget
{
        Q_OBJECT
    public:
        FunctionViewer( QWidget *parent = 0, const QGLWidget *shareWidget = 0, Qt::WindowFlags f = 0 );
        virtual ~FunctionViewer();

    public slots:
        void setViewModeWireframe( bool w );
        void setViewModePoint( bool w );
        bool getViewModeWireframe();
        bool getViewModePoint();

        void showMiniMap( bool w );
        bool isMiniMapEnabled();

        void changeFunctionMinColor();
        void setFunctionMinColor( QColor color );
        void changeFunctionMaxColor();
        void setFunctionMaxColor( QColor color );

        void changeBackgroundColor();
        void setBackgroundColor( QColor color );

        void changePointColor();
        void setPointColor( QColor color );

        void setFunction( const Function &func );
        void regenerateFunction();

        void setRangeX( double min, double max, double step );
        void setRangeY( double min, double max, double step );

        void setEnabled( bool w );

        void showSwarm( bool w );
        bool isSwarmEnabled();
        void setSwarm( Swarm<Function> *s );
        Swarm<Function> *getSwarm();

#ifdef USE_FTGL
        void showAxis( bool b );
        bool isFTGLFontLoaded();
#endif

        void changePointSize();
        void setPointSize( double size );
        double getPointSize();

        void setDefaultView();
        void showMiniMapGlobalBest( bool w );
        void showGlobalBest3D( bool w );

        void showTraceParticles( bool w );
        bool isParticleTracingEnabled();
        std::vector<std::vector<Vector<double> > > &getTraceParticleContainer();

    signals:
        void particleNumberChanged();

    protected:
        enum TListsState {GLLIST_DEFINED, GLLIST_UNDEFINED, GLLIST_REBUILD};

        void initializeGL();
        void resizeGL( int w, int h );
        void paintGL();

        void drawSwarm( bool draw_on_minimap = false );
        void drawMiniMap();
        void genereateOpenglLists();

#ifdef USE_FTGL
        void drawAxis();
        void drawXYZAxis( char axis );
        FTGLPolygonFont *ftgl_polygonfont;
#endif

        void mousePressEvent( QMouseEvent *event );
        void mouseMoveEvent( QMouseEvent *event );
        void wheelEvent( QWheelEvent *event );
        void mouseReleaseEvent( QMouseEvent *event );

        void calculateColor( double value, double &r, double &g, double &b );
        double evaluate( double x, double y );
        void calculateMiniMapLengths( double &x_length_, double &y_length_ );

        Vector<double>              viewport_translate;
        Vector<double>              viewport_rotate;

        Vector<double>              function_plot_range_x;          //[0]->min,[1]->max,[2]->step
        Vector<double>              function_plot_range_y;          //[0]->min,[1]->max,[2]->step

        VectorN<double>             mini_map_position;
        VectorN<double>             mini_map_size;                   //[0]->width,[1]->height as value from 0 to 1 where 1 corresponds to width or height
        bool                        mini_map_mouse_pressed_translate;
        bool                        mini_map_mouse_pressed_resize;
        bool                        mini_map;
        bool                        mini_map_mark_global_best;

        bool                        mark_global_best_3d;

        bool                        view_mode_wireframe;
        bool                        view_mode_points;

        double                      function_color_min_value[3];
        double                      function_color_max_value[3];
        double                      viewport_background_color[3];
        double                      swarm_point_color[3];

        QPoint                      mouse_last_position;

        Function                    function;
        std::pair<TListsState, GLuint>   opengl_lists;

        bool                        swarm_show;
        Swarm<Function>             *swarm;


#ifdef USE_FTGL
        bool                        axis_show;
        double                      axis_text_scale;
        double                      axis_line_length;
        double                      axis_number_distance;
        double                      axis_label_distance;
#endif

        bool                        trace_particles;
        std::vector<std::vector<Vector<double> > > trace_particles_container;

        double                      particle_point_size;

        bool                        content_enabled;

    private:
        double                      function_min_value;
        double                      function_max_value;
};

#endif
