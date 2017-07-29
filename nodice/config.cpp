/**
 * @file nodice/config.cpp
 * @brief Implemntation of the nodice/config module.
 */
/*
 * Copyright 2009,2013,2017 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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

#include <cstdlib>
#include <cstring>
#include <iostream>

#ifndef NODICE_SRC_DIR
# define NODICE_SRC_DIR "./"
#endif


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


  static std::vector<std::string>
  split_path_on_colon(std::string const& path)
  {
    std::vector<std::string> v;
    std::string::size_type p = 0;
    std::string::size_type q = path.find(':', p);
    while (true)
    {
      std::string s = path.substr(p, q - p);
      if (s.length() > 0)
        v.push_back(s);
      if (q == std::string::npos)
        break;
      p = q+1;
      q = path.find(':', p);
    }
    return v;
  }

  static std::vector<std::string>
  get_asset_search_path()
  {
    std::vector<std::string> search_path = {
      NODICE_SRC_DIR "/assets"
    };

    char* env = getenv("NODICE_ASSET_PATH");
    if (env)
    {
      for (auto const& p: split_path_on_colon(env))
      {
        search_path.push_back(p);
      }
    }

    return search_path;
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
: is_dirty_(false)
, is_debug_mode_(false)
, is_fullscreen_(false)
, is_small_window_(false)
, screen_width_(640)
, screen_height_(480)
, board_size_(8)
, asset_search_path_(get_asset_search_path())
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
          is_debug_mode_ = true;
          break;
        }

        case 'f':
        {
          is_fullscreen_ = true;
          break;
        }

        case 'w':
        {
          is_small_window_ = true;
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
is_debug_mode() const
{
  return is_debug_mode_;
}


bool NoDice::Config::
is_fullscreen() const
{
  return is_fullscreen_;
}


bool NoDice::Config::
is_small_window() const
{
  return is_small_window_;
}


int NoDice::Config::
screen_width() const
{
  return screen_width_;
}


void NoDice::Config::
set_screen_width(int w)
{
  if (screen_width_ != w)
  {
    screen_width_ = w;
    set_dirty();
  }
}


int NoDice::Config::
screen_height() const
{
  return screen_height_;
}


void NoDice::Config::
set_screen_height(int h)
{
  if (screen_height_ != h)
  {
    screen_height_ = h;
    set_dirty();
  }
}


int NoDice::Config::
board_size() const
{ return board_size_; }


void NoDice::Config::
set_board_size(int size)
{
  if (board_size_ != size)
  {
    board_size_ = size;
    set_dirty();
  }
}


std::vector<std::string> const& NoDice::Config::
asset_search_path() const
{
  return asset_search_path_;
}


void NoDice::Config::
set_dirty()
{
  is_dirty_ = true;
}
