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

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <ostream>
#include <string>
#include <sstream>
#include <map>
#include <utility>
#include <exception>

#define EXCEPTION_INFO __PRETTY_FUNCTION__,__FILE__,__LINE__,Exception::backtrace()
#define WARNING(msg) cerr << BLUE << "Warning:" << NORMAL << endl << "\t" << GREEN << __FILE__ <<  ":" << __LINE__ << NORMAL << endl << "\t" << __PRETTY_FUNCTION__ << endl << "\t" << RED << (msg) << NORMAL << endl

#define CLR_LINE    "\x1B[A"
#define NORMAL      "\x1B[0m"
#define CLR_SCR     "\033[2J\033[0;0f"
#define RED         "\x1B[31m"
#define GREEN       "\x1B[32m"
#define BLUE        "\x1B[34m"
#define YELLOW      "\x1B[33m"


class Exception : public std::exception
{
    public:
        Exception( const Exception &err );
        Exception( std::string msg = std::string() , std::string function = std::string(), std::string file = std::string(), int line = 0, std::string backtrace = std::string() );
        virtual ~Exception() throw();

        const std::string &getMessage() const;
        const std::string &getFunction() const;
        const std::string &getFile() const;
        int getLine() const;

        void printError() const;
        const std::string createErrorString() const;
        friend std::ostream &operator << ( std::ostream &stream, Exception &err );

        const char *what() const throw();

        static std::string backtrace( unsigned int max_depth = 100 );
        static std::map<std::string, std::string> parceBacktraceString( char *str );

    protected:
        std::string message;
        std::string function_name;
        std::string file_name;
        int         line_number;
        std::string backtrace_string;
};

class RuntimeError : public Exception
{
    public:
        RuntimeError( std::string msg = std::string() , std::string function = std::string(), std::string file = std::string(), int line = 0, std::string backtrace = std::string() ) : Exception( msg, function, file, line, backtrace )
        {
        }

        RuntimeError( const RuntimeError &err ) : Exception( err )
        {

        }

        virtual ~RuntimeError() throw()
        {
        }
};
#define Exception(msg) Exception((msg),EXCEPTION_INFO)
#define RuntimeError(msg) RuntimeError((msg),EXCEPTION_INFO)

#endif
