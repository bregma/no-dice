#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
#
# Copyright 2009 Stephen M. Webb  <stephen.webb@bregmasoft.com>
#
AC_PREREQ(2.61)
AC_COPYRIGHT([Copyright 2009 Stephen M. Webb  <stephen.webb@bregmasoft.com>])
AC_INIT(no-dice, 1.0, stephen.webb@bregmasoft.com)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR(config)
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([gnu 1.9 -Wno-syntax -Wall])
AC_CONFIG_HEADER([nodice_config.h])

# Checks for tools.
AC_PROG_CXX

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# Checks for third-party packages
PKG_CHECK_MODULES([SDL],      [sdl >= 1.2.11])
PKG_CHECK_MODULES([FREETYPE], [freetype2 >= 9.10.3])

AC_CONFIG_FILES(Makefile)
AC_CONFIG_FILES(src/Makefile)
AC_CONFIG_FILES(assets/Makefile)
AC_OUTPUT