/**
 * @file nodice/config.cpp
 * @brief Implemntation of the nodice/config module.
 */
/*
 * Copyright 2009-2013 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
 *
 * This file is part of no-dice.
 *
 * No-dice is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * No-dice is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with no-dice.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "nodice/config.h"

#include <cstring>
#include <iostream>


namespace
{
  /**
   * Tries to extract an option argument.
   * @param[in]  a1
   * @param[in]  a2
   * @param[out] index
   */
  static char const*
  getarg(char const* a1, char const* a2, int& index)
  {
    if (std::strlen(a1) > 0)
    {
      return a1;
    }
    if (a2)
    {
      ++index;
      return a2;
    }
    return NULL;
  }
}


/**
 * @param[in] argc Number of command-line arguments.
 * @param[in] argv Vector of command-line argument strings.
 *
 * Parses the command line arguments and sets variaous configurable items
 * appropriately.
 *
 * This contains a local reimplementation of getopt(3) because not all target
 * platforms support the POSIX API.
 */
NoDice::Config::
Config(int argc, char* argv[])
: isDirty_(false)
, isDebugMode_(false)
, isFullscreen_(false)
, isSmallWindow_(false)
, screenWidth_(640)
, screenHeight_(480)
, boardSize_(8)
{
  for (int i = 0; i < argc; ++i)
  {
    if (*argv[i] == '-')
    {
      char c = *(argv[i] + 1);
      switch (c)
      {
        case 'd':
        {
          isDebugMode_ = true;
          break;
        }

        case 'f':
        {
          isFullscreen_ = true;
          break;
        }

        case 'w':
        {
          isSmallWindow_ = true;
          break;
        }

        case 't':
        {
          char const* opt = getarg(argv[i]+2, (i < argc) ? argv[i+1] : NULL, i);
          if (opt == NULL)
          {
            std::cerr << "error parsing arg -t\n";
            break;
          }
          std::cerr << "arg t opt '" << opt << "'\n";
          break;
        }
      }
    }
  }
}


NoDice::Config::
~Config()
{
}


bool NoDice::Config::
isDebugMode() const
{
  return isDebugMode_;
}


bool NoDice::Config::
isFullscreen() const
{
  return isFullscreen_;
}


bool NoDice::Config::
isSmallWindow() const
{
  return isSmallWindow_;
}


int NoDice::Config::
screenWidth() const
{
  return screenWidth_;
}


void NoDice::Config::
setScreenWidth(int w)
{
  if (screenWidth_ != w)
  {
    screenWidth_ = w;
    setDirty();
  }
}


int NoDice::Config::
screenHeight() const
{
  return screenHeight_;
}


void NoDice::Config::
setScreenHeight(int h)
{
  if (screenHeight_ != h)
  {
    screenHeight_ = h;
    setDirty();
  }
}


int NoDice::Config::
boardSize() const
{ return boardSize_; }


void NoDice::Config::
setBoardSize(int size)
{
  if (boardSize_ != size)
  {
    boardSize_ = size;
    setDirty();
  }
}


void NoDice::Config::
setDirty()
{
  isDirty_ = true;
}
