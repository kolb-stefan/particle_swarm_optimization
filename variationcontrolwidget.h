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

#ifndef STATISTICCONTROLWIDGET_H
#define STATISTICCONTROLWIDGET_H

#include <QWidget>
#include "mainwindow.h"

class VariationControlWidget : public QWidget
{
        Q_OBJECT
    public:
        VariationControlWidget( MainWindow *mw, QWidget *parent = 0, Qt::WindowFlags f = 0 );
        ~VariationControlWidget();

        unsigned int getAverageNumber();
        double getFromValue();
        void setFromValue( double value );
        double getStepValue();
        double getOldFromValue();
        double getToValue();
        QMap<QString, QString> &getVariationVariableNames();
        QString getCurrentlyUsedVariable();


    public slots:
        void changeVariation( const QString &str );
        void startVariation();

        void enableTimer();
        void disableTimer();

    private:
        MainWindow          *ui_mainwindow;

        QComboBox           *ui_variable_select;
        QDoubleSpinBox      *ui_from_value;
        QDoubleSpinBox      *ui_to_value;
        QDoubleSpinBox      *ui_step;
        QSpinBox            *ui_average_number;
        QPushButton         *ui_start;

        QMap<QString, QString> variable_names;

        double              old_from_value;

};

#endif // STATISTICCONTROLWIDGET_H
