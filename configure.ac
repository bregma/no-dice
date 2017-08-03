#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
#
# Copyright 2009, 2010, 2011 Stephen M. Webb  <stephen.webb@bregmasoft.com>
#
AC_PREREQ(2.65)
AC_COPYRIGHT([
  Copyright 2010, 2011 Stephen M. Webb  <stephen.webb@bregmasoft.com>
  ])
AC_INIT([no-dice],
        [0.3.0],
        [stephen.webb@bregmasoft.com])
AC_CONFIG_MACRO_DIR(config.m4)
AC_CONFIG_AUX_DIR(config.aux)
AC_REQUIRE_AUX_FILE([tap-driver.sh])

AC_CONFIG_HEADER([nodice_config.h])

AM_INIT_AUTOMAKE([foreign 1.11 -Wno-syntax -Wall])
AM_SILENT_RULES([yes])

# Checks for tools.
AC_PROG_AWK
AC_PROG_CXX
AC_LANG([C++])
AM_PROG_AR
LT_PREREQ([2.4.2])
LT_INIT

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# Checks for third-party packages
PKG_CHECK_MODULES([SDL],      [sdl2 >= 2.0.4])
PKG_CHECK_MODULES([FREETYPE], [freetype2 >= 9.10.3])
PKG_CHECK_MODULES([GL],       [gl >= 7])

# Use SDL or EGL for video context?
AC_ARG_WITH(wm,
            AC_HELP_STRING([--with-wm=@<:@sdl|egl@:>@],
                           [selects the window manager]),
            nd_wm=$with_wm)
if test "x$nd_wm" = "xegl"; then
	AC_CHECK_HEADERS([GLES/egl.h],, AC_MSG_ERROR([missing EGL headers]))
	AC_DEFINE([HAVE_EGL], [1], [Use EGL for the video context])
	AC_DEFINE([HAVE_OPENGL_ES], [1], [Use OpenGL ES])
	GL_LIBS="-lgles11 -legl13"
fi
AM_CONDITIONAL([HAVE_EGL], [test "x$nd_wm" = "xegl"])
AC_SUBST([GL_CFLAGS])
AC_SUBST([GL_LIBS])

# Crank the warnings level up to 11
AC_SUBST([AM_CXXFLAGS],
         ["-Wall -Wextra -Werror -pedantic -std=c++14 -D_GNU_SOURCE=1"])
AC_DEFINE([NODICE_UNUSED],
          [__attribute__((unused))],[symbol is unused])

AC_CONFIG_FILES(Makefile)
AC_CONFIG_FILES(nodice/Makefile)
AC_CONFIG_FILES(assets/Makefile)
AC_CONFIG_FILES(tests/Makefile)
AC_OUTPUT
