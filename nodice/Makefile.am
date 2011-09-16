##
## Makefile for the no-dice project src directory.
##
## Copyright 2009, 2010 Stephen M. Webb  <stephen.webb@bregmasoft.com>
##
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, Version 2 of the License.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
##

AM_CXXFLAGS = -W -Wall -Wextra 

gamedir = ${prefix}/games

game_PROGRAMS = no-dice

if HAVE_EGL
vcontext_SOURCES = videocontextegl.h videocontextegl.cpp
else
vcontext_SOURCES = videocontextsdl.h videocontextsdl.cpp
endif

no_dice_SOURCES = \
	main.cpp \
	app.h              app.cpp \
	board.h            board.cpp \
	colour.h           colour.cpp \
	config.h           config.cpp \
	d4.h               d4.cpp \
	d6.h               d6.cpp \
	d8.h               d8.cpp \
	d12.h              d12.cpp \
	d20.h              d20.cpp \
	font.h             font.cpp \
	gamestate.h        gamestate.cpp \
	introstate.h       introstate.cpp \
	maths.h \
	object.h           object.cpp \
	opengl.h           opengl.cpp \
	playstate.h        playstate.cpp \
	shape.h            shape.cpp \
	video.h            video.cpp \
	videocontext.h \
	$(vcontext_SOURCES)

no_dice_CXXFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/include \
	-DHAVE_GL_VERTEX_BUFFERS \
	-DDATA_DIR=\"${pkgdatadir}\" \
	$(SDL_CFLAGS) \
	$(FREETYPE_CFLAGS) \
	$(GL_CFLAGS)

no_dice_LDADD = \
	$(SDL_LIBS) \
	$(FREETYPE_LIBS) \
	$(GL_LIBS)


