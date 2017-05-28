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

#ifndef FUNCTIONWIDGET_H
#define FUNCTIONWIDGET_H

#include <QWidget>
#include <QtGui>

#include "mainwindow.h"
#include "vectorn.h"
/**
    Widget to edit/manage/load expressions (functions). It is also possible
    to change the range in which the function is plotted. The selected expression
    is also used for the swarm optimization.
*/
class FunctionOptionsWidget : public QWidget
{
        Q_OBJECT
    public:
        FunctionOptionsWidget( MainWindow *mw, QWidget *parent = 0, Qt::WindowFlags f = 0 );
        virtual ~FunctionOptionsWidget();

        void getFunctionRange2D( VectorN<double> &min, VectorN<double> &max );
        void getFunctionRange( VectorN<double> &min, VectorN<double> &max );

    public slots:
        void showFunctionEditorDialog();
        void showFunctionManagerDialog();
        void changeFunctionRange();
        void setFunction();

        void modelitemChanged( QStandardItem *item );
        void fillRangeTable( size_t num, double dmin = -5.0, double dmax = 5.0 );

        void changeApplicationMode( PSOMode mode );

    protected:
        MainWindow          *ui_mainwindow;

        QPushButton         *ui_set_function;
        QLineEdit           *ui_current_expression;
        QTableView          *ui_high_dim_range_view;
        QStandardItemModel  *ui_high_dim_range_itemmodel;
        QGroupBox           *ui_container_groupbox;
        QPushButton         *ui_show_function_manager;
        QPushButton         *ui_show_function_editor;
        QDoubleSpinBox      *ui_low_dim_range_xmin;
        QDoubleSpinBox      *ui_low_dim_range_xmax;
        QDoubleSpinBox      *ui_low_dim_range_xstep;
        QDoubleSpinBox      *ui_low_dim_range_ymin;
        QDoubleSpinBox      *ui_low_dim_range_ymax;
        QDoubleSpinBox      *ui_low_dim_range_ystep;
};

#endif // FUNCTIONWIDGET_H
