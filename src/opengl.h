/**
 * @file opengl.h
 * @brief Portable wrapper for OpenGL/OpenGL ES inclusion.
 *
 * Copyright 2009 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of Version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef NO_DICE_OPENGL_H
#define NO_DICE_OPENGL_H 1

#if defined(_WIN32) && !defined(__CYGWIN__)
# define WIN32_LEAN_AND_MEAN 1
# include <windows.h>
#endif
#ifdef min
# undef min
#endif
#ifdef max
# undef max
#endif

#ifdef HAVE_OPENGL_ES
# include <GLES/gl.h>
#else
# include <GL/gl.h>
#endif

#endif // NO_DICE_OPENGL_H
