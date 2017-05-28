# Copyright (C) 2008-2013 Stefan Kolb.
# 
# This file is part of the program pso (particle swarm optimization).
# 
# The program pso is free software: you can redistribute it
# and/or modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation, either
# version 2 of the License, or (at your option) any later version.
# 
# The program pso is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with pso. If not, see <http://www.gnu.org/licenses/>.


project(pso)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")

find_package(Qt4 REQUIRED)
find_package(Qwt REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
find_package(FTGL REQUIRED)
find_package(MuParser REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${FREETYPE_INCLUDE_DIR_freetype2})

add_definitions(-DUSE_FTGL)

set(pso_source function.cpp graphwidget.cpp particleviewwidget.cpp variationcontrolwidget.cpp functionoptionswidget.cpp swarmcontrolwidget.cpp main.cpp mainwindow.cpp dockmanager.cpp dockwidget.cpp exception.cpp subprocess.cpp functioneditdialog.cpp functionmanagerdialog.cpp functionviewer.cpp particle.cpp)

set(pso_moc_header mainwindow.h dockmanager.h dockwidget.h functioneditdialog.h functionmanagerdialog.h functionviewer.h swarmcontrolwidget.h functionoptionswidget.h variationcontrolwidget.h particleviewwidget.h graphwidget.h)
qt4_wrap_cpp (pso_moc_outfiles ${pso_moc_header})

# qt4_automoc(${pso_source})
add_executable(pso ${pso_source} ${pso_moc_outfiles})
target_link_libraries(pso ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTOPENGL_LIBRARY} ${QWT_LIBRARY} ${OPENGL_LIBRARY} ${FTGL_LIBRARY} ${MUPARSER_LIBRARIES})

install(TARGETS pso RUNTIME DESTINATION bin)
