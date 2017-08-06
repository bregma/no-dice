/**
 * @file nodice/board.h
 * @brief Public interface of the nodice/board module.
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
#ifndef NODICE_BOARD_H
#define NODICE_BOARD_H 1

#include "nodice/maths.h"
#include "nodice/object.h"
#include <utility>
#include <vector>


namespace NoDice
{
  class Config;

  /**
   * The playing surface.
   */
  class Board
  {
  public:
    Board(Config const* config);

    ObjectPtr const&
    at(int x, int y) const;

    void
    update();

    void
    draw() const;

    void
    start_swap(ivec2 objPos1, ivec2 objPos2);

    void
    un_swap();

    bool
    is_swapping() const;

    bool
    is_replacing() const;

    ObjectBrace
    find_wins();

  private:
    ObjectPtr& at(const ivec2& point);

  private:
    using RemovalQueue = std::vector<ivec2>;
    using MovePair     = std::pair<ivec2, ivec2>;
    using FallingQueue = std::vector<MovePair>;
    using CreateQueue  = std::vector<ivec2>;

    enum State
    {
      state_idle,
      state_swapping,
      state_removing,
      state_falling
    };

    Config const*  config_;
    ObjectBag      objects_;
    State          state_;
    float          swap_step_;
    ivec2          swap_obj_[2];
    RemovalQueue   removal_queue_;
    FallingQueue   falling_queue_;
    CreateQueue    create_queue_;
  };
} // namespace NoDice

#endif // NODICE_BOARD_H
