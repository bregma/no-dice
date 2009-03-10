##
## Makefile for the no-dice project src directory.
##
## Copyright 2009 Stephen M. Webb  <stephen.webb@bregmasoft.com>
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
datadir = /usr/share/games/gljewel/data


bin_PROGRAMS = no-dice

no_dice_SOURCES = \
	main.cpp \
	nodice/app.h nodice/app.cpp \
	nodice/config.h nodice/config.cpp \
	nodice/video.h nodice/video.cpp

no_dice_CXXFLAGS = \
	$(SDL_CFLAGS) \
	$(FREETYPE_CFLAGS)

no_dice_LDADD = \
	$(SDL_LIBS) \
	$(FREETYPE_LIBS) \
	-lGL


