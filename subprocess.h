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

#ifndef SUBPROCESS_H
#define SUBPROCESS_H

#include <string>
#include <ostream>
#include <istream>

#include <ext/stdio_sync_filebuf.h>
#include <ext/stdio_filebuf.h>

class Subprocess
{
    public:
        Subprocess( std::string command );
        virtual ~Subprocess();

        void wait();

        std::istream &getStdout();
        std::ostream &getStdin();

        static std::string execute( std::string command );

    private:
        Subprocess( const Subprocess &other ) {}
        Subprocess &operator=( const Subprocess &other ) {return *this;}
        bool operator==( const Subprocess &other ) const {return false;}

        pid_t   pid;
        int     pipe_read[2];
        int     pipe_write[2];

        /**
            for informations about how to create std::istream and std::outstream from file descriptors look at
            file $GCC_SOURCE/libstdc++-v3/src/ios_init.cc . In this file cout,cin,... is initialised.
        */
        std::istream *stdout;
        std::ostream *stdin;

        __gnu_cxx::stdio_sync_filebuf<char> *buf_stdin_sync;
        __gnu_cxx::stdio_sync_filebuf<char> *buf_stdout_sync;
};

#endif // SUBPROCESS_H
