/**
 * @file nodice/playstate.h
 * @brief Public interface of the nodice/playstate module.
 */
/*
 * Copyright 2010,2011,2017 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
#ifndef NODICE_PLAYSTATE_H
#define NODICE_PLAYSTATE_H 1

#include "nodice/gamestate.h"

#include "nodice/board.h"
#include "nodice/maths.h"
#include <string>
#include <vector>


namespace NoDice
{
  class Font;

  class PlayState
  : public GameState
  {
  public:
    PlayState(Config const* config);
    ~PlayState();

    void pointerMove(int x, int y, int dx, int dy);
    void pointerClick(int x, int y, PointerAction action);

    void update(App& app);
    void draw(Video& video);

  private:
    void calculateScore(const ObjectBrace& matches);

  private:
    enum SubState
    {
      state_idle,
      state_swapping,
      state_unswapping,
      state_replacing,
      state_rotating,
      state_end
    };

    SubState                  state_;
    Board                     gameboard_;
    Font&                     score_font_;
    bool                      mouse_is_down_;
    Vector2i                  mouse_down_pos_;
    Vector2i                  selected_pos_;
    int                       multiplier_;
    int                       score_;
    std::vector<std::string>  win_messages_;
    Matrix4f                  unproject_;
  };

} // namespace noDice

#endif // NODICE_PLAYSTATE_H
