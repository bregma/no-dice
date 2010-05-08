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

bindir = /usr/games
datadir = /usr/share/games/no-dice/data


bin_PROGRAMS = no-dice

if HAVE_EGL
vcontext_SOURCES = videocontextegl.h videocontextegl.cpp
else
vcontext_SOURCES = videocontextsdl.h videocontextsdl.cpp
endif

no_dice_SOURCES = \
	main.cpp \
	app.h              app.cpp \
	config.h           config.cpp \
	font.h             font.cpp \
	gamestate.h        gamestate.cpp \
	introstate.h       introstate.cpp \
	object.h           object.cpp \
	playstate.h        playstate.cpp \
	shape.h            shape.cpp \
	video.h            video.cpp \
	videocontext.h \
	$(vcontext_SOURCES)

no_dice_CXXFLAGS = \
	-I$(top_srcdir)/include \
	$(SDL_CFLAGS) \
	$(FREETYPE_CFLAGS) \
	$(GL_CFLAGS)

no_dice_LDADD = \
	$(SDL_LIBS) \
	$(FREETYPE_LIBS) \
	$(GL_LIBS)


