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

#include "muParser.h"
#include "function.h"

Function::Function() : parser( new mu::Parser )
{

}

/**
    To copy the object it is enougth to use the same expression
    then the object \a other.

    \param[in] other
*/
Function::Function( const Function &other ) : parser( new mu::Parser )
{
    setExpression( other.getExpression() );
}

Function &Function::operator=( const Function &other )
{
    delete parser;
    parser = new mu::Parser;
    setExpression( other.getExpression() );
}

Function::~Function()
{
    delete parser;
}

/**
    Sets the expression and checks if the right variable names are used. The variable names
    are x1, x2, x3 and so on. The variables are maped to the vector \a variables and by
    changing the for example variables[0] the variable x1 is changed. To evaluate the function
    and setting the variables the functions operator()(...) are used.

    \param[in]  expr    contains the function to parce
*/
void Function::setExpression( const std::string &expr )
{
    try
    {
        clear();

        parser->DefineFun<double( * )( double, double ) >( "pow", std::pow );

        parser->SetExpr( expr );

        mu::varmap_type used_variables = parser->GetUsedVar();

        //Check if the variable names are correct set to x1, x2, x3, ...
        for( unsigned int i = 0; i < used_variables.size(); ++i )
        {
            std::stringstream sstream;
            sstream << "x" << i + 1;

            std::string var_name( sstream.str() );

            if( used_variables.find( var_name ) == used_variables.end() )
            {
                std::string vars;

                for( mu::varmap_type::const_iterator it = used_variables.begin(); it != used_variables.end(); ++it )
                {
                    vars += it->first + ", ";
                }

                vars.resize( vars.size() - 2 );
                throw RuntimeError( std::string( "Function should only contain variables like x1, x2, x3,... but contains " ) + vars + std::string( " !" ) );
            }
        }

        variables.resize( used_variables.size() );

        //map variables to the vector<double> variables
        for( unsigned int i = 0; i < used_variables.size(); ++i )
        {
            std::stringstream sstream;
            sstream << "x" << i + 1;

            std::string var_name( sstream.str() );
            parser->DefineVar( var_name, &variables[i] );
        }
    }
    catch( mu::Parser::exception_type &e )
    {
        throw RuntimeError( e.GetMsg() );
    }
}

std::string Function::getExpression() const
{
    return parser->GetExpr();
}

/**
    Returns the actually used nuber of variables in the current expression.
*/
std::size_t Function::getNumberOfVariablesInExpression() const
{
    return parser->GetUsedVar().size();
}

void Function::clear()
{
    parser->ClearVar();
}

bool Function::isEmpty()
{
    return parser->GetExpr().empty();
}

/**
    Evaluates the function at position x and returns the result

    \param[in] x
*/
double Function::operator()( VectorN< double > &x )
{
    if( x.size() >= variables.size() )
    {
        for( unsigned int i = 0; i < variables.size(); ++i )
        {
            variables[i] = x[i];
        }

        return parser->Eval();
    }
    else
    {
        throw RuntimeError( "Error evaluating function: to few variables given in x!" );
    }
}

double Function::operator()( double x )
{
    VectorN< double > tmp_x( 1 );
    tmp_x[0] = x;
    return this->operator()( tmp_x );
}

double Function::operator()( double x, double y )
{
    VectorN< double > tmp_x( 2 );
    tmp_x[0] = x;
    tmp_x[1] = y;
    return this->operator()( tmp_x );
}

double Function::operator()( double x, double y, double z )
{
    VectorN< double > tmp_x( 3 );
    tmp_x[0] = x;
    tmp_x[1] = y;
    tmp_x[2] = z;
    return this->operator()( tmp_x );
}

std::vector< std::string > split_string( const std::string &str, const std::string &split )
{
    std::vector<std::string> result;
    std::string tmp;
    size_t pos = 0;

    if( split.empty() ) {return result;}

    while( pos != std::string::npos )
    {
        size_t old_pos = pos;
        pos = str.find( split, old_pos );

        if( pos != std::string::npos )
        {
            tmp = str.substr( old_pos, pos - old_pos );
            result.push_back( tmp );

            pos += split.size();

            if( pos >= str.size() )
            {
                break;
            }
        }
        else
        {
            tmp = str.substr( old_pos, str.size() - old_pos );
            result.push_back( tmp );
            break;
        }
    }

    return result;
}

std::string trim_string( const std::string &str )
{
    std::string result = str;
    std::string::size_type num_trim_chars = 0;

    for( std::string::size_type i = 0; i < result.size(); ++i )
    {
        if( result[i] == ' ' )
        {
            num_trim_chars++;
        }
        else
        {
            break;
        }
    }

    result.erase( 0, num_trim_chars );

    num_trim_chars = 0;

    for( std::string::size_type i = result.size() - 1; i >= 0 ; --i )
    {
        if( result[i] == ' ' )
        {
            num_trim_chars++;
        }
        else
        {
            break;
        }
    }

    result.erase( result.size() - num_trim_chars, num_trim_chars );

    return result;
}

std::string::size_type find_in_expression( const std::string &expr, const std::string &name )
{
    std::string expression = std::string( " " ) + expr + std::string( " " );
    std::string delimiters = ",^+-*/() ";
    std::string::size_type current_position = expression.find( name );

    if( current_position == std::string::npos )
    {
        return std::string::npos;
    }
    else
    {
        bool begin_delimiter_found = false, end_delimiter_found = false;

        for( std::string::iterator it = delimiters.begin(); it != delimiters.end(); ++it )
        {
            if( expression[current_position - 1] == *it )
            {
                begin_delimiter_found = true;
            }

            if( expression[current_position + name.size()] == *it )
            {
                end_delimiter_found = true;
            }
        }

        if( begin_delimiter_found && end_delimiter_found )
        {
            return current_position - 1;
        }
        else
        {
            return std::string::npos;
        }
    }

    return current_position - 1;
}

/**
    If a listing like "pi=3.14;alpha=55;alpha*pi*x1" is given the function
    returns a string where all constants are replaced by ist value. For this
    example the result is "55*3.14*x1"

    Note: Only numeric definition are allowed

    \param[in] listing
*/
std::string Function::reduceListingToExpression( std::string listing )
{
    std::map<std::string, std::string> consts_map;
    std::string expression;

    //extracting definitions of constants and the used expression
    {
        std::vector<std::string> lines = split_string( listing, ";" );

        for( std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it )
        {
            std::vector<std::string> constant = split_string( *it, "=" );

            if( constant.size() < 2 ) //expression is found
            {

                if( expression.empty() )
                {
                    expression = *it;
                }
                else
                {
                    throw RuntimeError( "Multiple funcions are defined in the listing!" );
                }
            }
            else if( constant.size() == 2 ) //definition of constant is found
            {
                consts_map[constant[0]] = std::string( "(" ) + constant[1] + std::string( ")" );
            }
            else
            {
                RuntimeError Error( "Wrong syntax!" );
            }
        }
    }

    if( expression.empty() )
    {
        throw RuntimeError( "No expression found in listing!" );
    }

    //replaceing the constants in the expression
    for( std::map<std::string, std::string>::const_iterator it = consts_map.begin(); it != consts_map.end(); ++it )
    {
        std::string::size_type current_position;
        std::string name = trim_string( it->first );
        std::string value = trim_string( it->second );

        while( ( current_position = find_in_expression( expression, name ) ) != std::string::npos )
        {
            expression.replace( current_position, name.size(), value );
        }
    }

    return trim_string( expression );
}

void FunctionTest()
{
    Function f;
//
//     f.setExpression("10*x1");
//     std::cout << f.getNumberOfVariablesInExpression() << std::endl;
//
//     std::cout << f(7.7) << std::endl;
//     std::cout << "|" <<trim_string(" aa a a ") << "|" << std::endl;
//
//     std::cout << f.reduceListingToExpression(" pi = 3.14; pii = 6.248; (pi*pii)*x1+10.4") << std::endl;
//
//     f.setExpression("5*abs(sin(2*sqrt(x1*x1+x2*x2))/sqrt(x1*x1+x2*x2))");
//     f.setExpression("abs(10)");


//     mu::Parser parser;
//     double x = 10.,y = 10.;
// //     parser.DefineVar("x1", &x);
// //     parser.DefineVar("x2", &y);
//
//     parser.ClearConst();
//     parser.ClearFun();
//     parser.ClearInfixOprt();
//     parser.ClearOprt();
//     parser.ClearPostfixOprt();
//     parser.ClearVar();
//
//     parser.SetExpr("5*abs(sin(2*sqrt(x1*x1+x2*x2))/sqrt(x1*x1+x2*x2))");
//     mu::varmap_type used_variables = parser.GetUsedVar();
//     for(mu::varmap_type::const_iterator it = used_variables.begin(); it != used_variables.end(); ++it)
//     {
//         std::cout << "Name: " << it->first << " Address: [0x" << it->second << "]\n";
//     }
//     std::cout << parser.Eval() << std::endl;
}

