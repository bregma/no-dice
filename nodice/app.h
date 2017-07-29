/**
 * @file nodice/app.h
 * @brief Public interface of the nodice/app module.
 */
/*
 * Copyright 2009,2010,2017 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
#ifndef NODICE_APP_H
#define NODICE_APP_H 1

#include "nodice/fontcache.h"
#include "nodice/gamestate.h"
#include "nodice/video.h"
#include <stack>
#include <vector>


namespace NoDice
{
  class Config;

  /**
   * A wrapper class to provide application-wide setup and teardown.
   */
  class App
  {
  public:
    explicit
    App(Config * config);

    ~App();

    int
    run();

    void
    push_game_state(GameStatePtr state);

    void
    pop_game_state();

    Config&
    config();

    FontCache&
    font_cache();

  private:
    /** @brief A special RAII object for SDL initialization and teardown. */
    struct SdlInit
    {
      SdlInit();
      ~SdlInit();
    };

  private:
    typedef std::stack<GameStatePtr, std::vector<GameStatePtr>> StateStack;

    Config*        config_;
    SdlInit        sdl_init_;
    Video          video_;
    FontCache      font_cache_;
    StateStack     state_stack_;
  };

} // namespace NoDice
#endif // NODICE_APP_H

