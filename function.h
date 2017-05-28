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

#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cmath>

#include "vectorn.h"

/**
    This is a simple wrapper for the muParser library
*/

namespace mu
{
    class Parser;
}

class Function
{
    public:
        Function();
        Function( const Function &other );
        virtual ~Function();

        Function &operator = ( const Function &other );

        void setExpression( const std::string &expr );
        std::string getExpression() const;
        std::size_t getNumberOfVariablesInExpression() const;

        void clear();
        bool isEmpty();

        double operator()( VectorN<double> &x );
        double operator()( double x );
        double operator()( double x, double y );
        double operator()( double x, double y, double z );

        static std::string reduceListingToExpression( std::string listing );
    protected:
        mu::Parser           *parser;
        std::vector<double>  variables;
};

void FunctionTest();

#endif // FUNCTION_H
