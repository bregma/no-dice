/**
 * @file nodice/config.h
 * @brief Public interface of the nodice/config module.
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
#ifndef NODICE_CONFIG_H
#define NODICE_CONFIG_H 1

#include <string>
#include <vector>


namespace NoDice
{
  /**
   * Application-wide configuration.
   */
  class Config
  {
  public:
    /** Construcrs a Config object from command-line arguments. */
    Config(int argc, char* argv[]);

    /** Destroys a Config object. */
    ~Config();

    /** Indicates if debug mode is enabled. */
    bool
    is_debug_mode() const;

    /** Indicates if fullscreen mode is active. */
    bool
    is_fullscreen() const;

    /** Indicates if (text mode) small window mode is set. */
    bool
    is_small_window() const;

    /** Gets the currently selected screen width (in pixels). */
    int
    screen_width() const;

    /** Sets the current screen width (in pixels). */
    void
    set_screen_width(int w);

    /** Gets the currently selected screen height (in pixels). */
    int
    screen_height() const;

    /** Sets the current screen height. */
    void
    set_screen_height(int h);

    /** Gets the board size (boards are always square). */
    int
    board_size() const;

    /** Sets the board size. */
    void
    set_board_size(int size);

    /** Gets the search path for assets. */
    std::vector<std::string> const&
    asset_search_path() const;

  private:
    void
    set_dirty();

  private:
    bool                     is_dirty_;
    bool                     is_debug_mode_;
    bool                     is_fullscreen_;
    bool                     is_small_window_;
    int                      screen_width_;
    int                      screen_height_;
    int                      board_size_;
    std::vector<std::string> asset_search_path_;
  };
} // namespace NoDice

#endif // NODICE_CONFIG_H
