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
    isDebugMode() const;

    /** Indicates if fullscreen mode is active. */
    bool
    isFullscreen() const;

    /** Indicates if (text mode) small window mode is set. */
    bool
    isSmallWindow() const;

    /** Gets the currently selected screen width (in pixels). */
    int
    screenWidth() const;

    /** Sets the current screen width (in pixels). */
    void
    setScreenWidth(int w);

    /** Gets the currently selected screen height (in pixels). */
    int
    screenHeight() const;

    /** Sets the current screen height. */
    void
    setScreenHeight(int h);

    /** Gets the board size (boards are always square). */
    int
    boardSize() const;

    /** Sets the board size. */
    void
    setBoardSize(int size);

    /** Gets the search path for assets. */
    std::vector<std::string> const&
    asset_search_path() const;

  private:
    void
    setDirty();

  private:
    bool                     isDirty_;
    bool                     isDebugMode_;
    bool                     isFullscreen_;
    bool                     isSmallWindow_;
    int                      screenWidth_;
    int                      screenHeight_;
    int                      boardSize_;
    std::vector<std::string> asset_search_path_;
  };
} // namespace NoDice

#endif // NODICE_CONFIG_H
