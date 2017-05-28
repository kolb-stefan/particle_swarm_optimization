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

#include <unistd.h>     //for function access
#include <string.h>
#include <execinfo.h>
#include <stdio.h>

#include <iostream>
#include <iomanip>
#include <map>
#include <cstring>
#include <vector>

#include <cxxabi.h>

#include "exception.h"
#undef Exception

#include "subprocess.h"

Exception::Exception( const Exception &err )
    : message( err.message ), function_name( err.function_name ), file_name( err.file_name ), line_number( err.line_number ), backtrace_string( err.backtrace_string )
{

}

Exception::Exception( std::string msg , std::string function, std::string file , int line, std::string backtrace )
    :  message( msg ), function_name( function ), file_name( file ), line_number( line ), backtrace_string( backtrace )
{

}

Exception::~Exception() throw()
{

}

const std::string &Exception::getFunction() const
{
    return function_name;
}

const std::string &Exception::getMessage() const
{
    return message;
}

const std::string &Exception::getFile() const
{
    return file_name;
}

int Exception::getLine() const
{
    return line_number;
}

void Exception::printError() const
{
    std::cout << createErrorString();
}

const std::string Exception::createErrorString() const
{
    std::ostringstream stream;
    stream << "Exception" << std::endl;
    stream << "\t" << GREEN << file_name <<  ":" << line_number << NORMAL << std::endl;
    stream << "\t" << function_name << std::endl;
    stream << "\t" << RED << message << NORMAL << std::endl;

    stream << std::endl;
    stream << backtrace_string << std::endl;

    return stream.str();
}

std::ostream &operator<<( std::ostream &stream, Exception &err )
{
    stream << err.createErrorString();
    return stream;
}

const char *Exception::what() const throw()
{
    static std::string cash = createErrorString();
    return cash.c_str();
}

/**
    Prints the current functions on the stack
*/
std::string Exception::backtrace( unsigned int max_depth )
{
    int nptrs = 0;
    std::stringstream sstream;
    char **strings = NULL;
    void **buffer = new void*[max_depth];

    if( buffer == NULL ) return std::string();

    nptrs = ::backtrace( buffer, max_depth );

    strings = backtrace_symbols( buffer, nptrs );

    std::vector<std::map<std::string, std::string> > backtace_info;

    for( int i = 0; i < nptrs; i++ )
    {
        if( i != 0 )
        {
            backtace_info.push_back( parceBacktraceString( strings[i] ) );
            std::map<std::string, std::string> &function_info = backtace_info.back();
            std::string addr2line_command = std::string( "addr2line -i -s -e " ) + std::string( " " ) + function_info["program"] + std::string( " " ) + function_info["function return adress"];
            std::string addr2line_result = Subprocess::execute( addr2line_command );

            if( !addr2line_result.empty() )
            {
                addr2line_result.erase( addr2line_result.size() - 1, 1 );

                if( addr2line_result.find( "??:0" ) == std::string::npos )
                {
                    function_info["file"] = addr2line_result;
                }
            }

//              sstream << function_info["function name"] << "\t" << function_info["function offset"] << "\t" << function_info["function return adress"] << std::endl;
//              sstream << addr2line_command << "\t" << addr2line_result << std::endl;
        }
    }

    unsigned int max_function_lenght = 20, function_max_print_lenght = 70;
    unsigned int max_file_lenght = 15;
    unsigned int max_program_lenght = 20;
    unsigned int max_return_adress_lenght = 16;

    for( std::vector<std::map<std::string, std::string> >::iterator it = backtace_info.begin(); it != backtace_info.end(); ++it )
    {
        std::map<std::string, std::string>::iterator find;

        if( ( find = it->find( "function name" ) ) != it->end() )
        {
            if( find->second.size() > max_function_lenght )
            {
                max_function_lenght = find->second.size();
            }
        }

        if( ( find = it->find( "file" ) ) != it->end() )
        {
            if( find->second.size() > max_file_lenght )
            {
                max_file_lenght = find->second.size();
            }
        }

        if( ( find = it->find( "program" ) ) != it->end() )
        {
            if( find->second.size() > max_program_lenght )
            {
                max_program_lenght = find->second.size();
            }
        }
    }

    if( max_function_lenght > function_max_print_lenght )
    {
        max_function_lenght = function_max_print_lenght;
    }

    max_function_lenght += 3;
    max_file_lenght += 3;
    max_program_lenght += 3;
    max_return_adress_lenght += 3;

//      sstream << max_function_lenght << "\t" << max_file_lenght  << "\t" <<  max_program_lenght << std::endl;

    sstream << "Functions on stack" << std::endl;
    sstream << "\t" << std::setw( max_file_lenght ) << std::left << "file" << std::setw( max_function_lenght ) << std::left << "function" << std::setw( max_return_adress_lenght ) << std::left << "return adress" << std::setw( max_program_lenght ) << std::left << "program" << std::endl;
    sstream << "\t" << std::setfill( '-' ) << std::setw( max_file_lenght + max_function_lenght + max_program_lenght + max_return_adress_lenght ) << "" << std::endl;
    sstream << std::setfill( ' ' );

    for( std::vector<std::map<std::string, std::string> >::iterator it = backtace_info.begin(); it != backtace_info.end(); ++it )
    {
        std::string function = ( *it )["function name"];
        std::string &file = ( *it )["file"];
        std::string &program = ( *it )["program"];
        std::string &return_adress = ( *it )["function return adress"];

        if( function.size() > function_max_print_lenght )
        {
            std::string::size_type pos = function.find( "(" );

            if( pos != std::string::npos )
            {
                function = function.substr( 0, pos );
            }
            else
            {
                function.erase( function_max_print_lenght - 3 );
                function.append( "..." );
            }
        }

        sstream << "\t" << std::setw( max_file_lenght ) << std::left << file << std::setw( max_function_lenght ) << std::left << function << std::setw( max_return_adress_lenght ) << std::left << return_adress << std::setw( max_program_lenght ) << std::left << program << std::endl;
    }

    delete [] buffer;
    delete [] strings;
    return sstream.str();
}

/**
This function parses a string (line) returned by the function backtrace_symbols() and
and returns the parced information as a map.

\param[in]  str
\param[in]  sstream
*/
std::map<std::string, std::string> Exception::parceBacktraceString( char *str )
{
    std::map<std::string, std::string> result;
    unsigned int string_length = std::strlen( str );
    char *buffer = NULL;
    char empty_string[2];
    empty_string[0] = ' ';
    empty_string[1] = 0;

    int demangle_status = 0;

    char *programm = NULL;
    char *function = NULL;
    char *real_function_name = NULL;
    char *function_offset = NULL;
    char *return_adress = NULL;

    if( string_length < 8192 )
    {
        buffer = new char[string_length];
    }
    else
    {
        return result;
    }

    std::memcpy( buffer, str, string_length );

    programm = buffer;

    for( unsigned int i = 0; i < string_length; ++i )
    {
        if( buffer[i] == '(' )
        {
            buffer[i] = '\0';
            function = &buffer[i + 1];
        }
        else if( buffer[i] == '+' )
        {
            buffer[i] = '\0';
            function_offset = &buffer[i + 1];
        }
        else if( buffer[i] == ')' )
        {
            buffer[i] = '\0';
        }
        else if( buffer[i] == '[' )
        {
            buffer[i] = '\0';
            return_adress = &buffer[i + 1];
        }
        else if( buffer[i] == ']' )
        {
            buffer[i] = '\0';
        }
    }

    if( programm == NULL ) programm = empty_string;

    if( function == NULL ) function = empty_string;

    if( function_offset == NULL ) function_offset = empty_string;

    if( return_adress == NULL ) return_adress = empty_string;

    real_function_name = abi::__cxa_demangle( function, 0, 0, &demangle_status );

    if( real_function_name )
    {
        result["function name"] = std::string( real_function_name );
        delete [] real_function_name;
    }
    else
    {
        result["function name"] = std::string( function );
    }

    result["function offset"] = std::string( function_offset );
    result["function return adress"] = std::string( return_adress );
    result["program"] = std::string( programm );

    delete [] buffer;
    return result;
}
