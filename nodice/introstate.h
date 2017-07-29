/**
 * @file introstate.h
 * @brief Public interface of the introstate module.
 */
/*
 * Copyright 2010 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
#ifndef IntroState_H
#define IntroState_H 1

#include "nodice/gamestate.h"
#include "nodice/maths.h"


namespace NoDice
{
  class Font;

  /**
   * Provides the "game introduction" state:  mostly just the loading screen.
   */
  class IntroState
  : public GameState
  {
  public:
    enum NextState
    {
      next_state_same,
      next_state_options,
      next_state_play,
      next_state_quit
    };

  public:
    IntroState(App* app, const Video& video);

    ~IntroState();

    void pause();
    void resume();

    void key(SDL_keysym keysym);
    void pointerMove(int x, int y, int dx, int dy);
    void pointerClick(int x, int y, PointerAction action);
    void update(App& app);

    void draw(Video& video);

  private:
    bool       is_active_;
    Font&      menu_font_;
    Vector2f   title_pos_;
    int        selected_;
    NextState  next_state_;
  };

} // namespace NoDice

#endif // IntroState_H
