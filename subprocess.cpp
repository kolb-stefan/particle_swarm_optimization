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

#include "subprocess.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdio>

#include <fstream>

#include "exception.h"

Subprocess::Subprocess( std::string command ) : stdout( NULL ), stdin( NULL )
{
    if( pipe( pipe_write ) != 0 )
    {
        throw RuntimeError( "Unable to create pipe" );
    }

    if( pipe( pipe_read ) != 0 )
    {
        throw RuntimeError( "Unable to create pipe" );
    }

    pid = fork();

    if( pid == 0 ) //child
    {
        close( pipe_write[1] );                     //closes the write-pipe
        close( pipe_read[0] );                      //closes the read-pile

        if( dup2( pipe_write[0], STDIN_FILENO ) == -1 ) //connects the read-pipe with the stdin
        {
            throw RuntimeError( "Function dup2 failed" );
        }

        if( dup2( pipe_read[1], STDOUT_FILENO ) == -1 ) //connects the write-pipe with the stdout
        {
            throw RuntimeError( "Function dup2 failed" );
        }

        system( command.c_str() );

        close( pipe_write[1] );                     //clean up the pipe
        close( pipe_read[0] );                      //clean up the pipe
        exit( 0 );
    }
    else//parent
    {
        close( pipe_write[0] );                     //closes the read-pile
        close( pipe_read[1] );                      //closes the write-pipe

        buf_stdin_sync = new __gnu_cxx::stdio_sync_filebuf<char>( fdopen( pipe_write[1], "w" ) );
        stdin = new std::ostream( buf_stdin_sync );

        buf_stdout_sync = new __gnu_cxx::stdio_sync_filebuf<char>( fdopen( pipe_read[0], "r" ) );
        stdout = new std::istream( buf_stdout_sync );
    }
}

Subprocess::~Subprocess()
{
    if( stdin ) delete stdin;

    if( stdout ) delete stdout;

    if( buf_stdin_sync ) delete buf_stdin_sync;

    if( buf_stdout_sync ) delete buf_stdout_sync;

    close( pipe_read[0] );
    close( pipe_write[1] );

    wait();
}

void Subprocess::wait()
{
    int status = 0;
    waitpid( pid, &status, 0 );
}

std::ostream &Subprocess::getStdin()
{
    return *stdin;
}

std::istream &Subprocess::getStdout()
{
    return *stdout;
}

std::string Subprocess::execute( std::string command )
{
    const unsigned int buffer_size = 1024;
    char buffer[buffer_size];
    std::string output;

    Subprocess subprocess( command );

    std::istream &is = subprocess.getStdout();

    while( is )
    {
        is.read( buffer, buffer_size );
        output.append( buffer, is.gcount() );
    }

    return output;
}
