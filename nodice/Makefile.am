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

gamedir = ${prefix}/games

game_PROGRAMS = no-dice

if HAVE_EGL
vcontext_SOURCES = videocontextegl.h videocontextegl.cpp
else
vcontext_SOURCES = videocontextsdl.h videocontextsdl.cpp
endif

noinst_LTLIBRARIES = libnodice.la

libnodice_la_SOURCES = \
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
	fontcache.h        fontcache.cpp \
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

libnodice_la_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/include \
	-DHAVE_GL_VERTEX_BUFFERS \
	-DDATA_DIR=\"${pkgdatadir}\" \
	-DNODICE_SRC_DIR=\"${abs_top_srcdir}\" \
	$(SDL_CFLAGS) \
	$(FREETYPE_CFLAGS) \
	$(GL_CFLAGS)

libnodice_la_LIBADD = \
	$(SDL_LIBS) \
	$(FREETYPE_LIBS) \
	$(GL_LIBS)

no_dice_SOURCES = \
	main.cpp

no_dice_CPPFLAGS = $(libnodice_la_CPPFLAGS)

no_dice_LDADD = \
	libnodice.la


