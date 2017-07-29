/**
 * @file nodice/board.cpp
 * @brief Implemntation of the no-dice board module.
 *
 * Copyright 2010, 2011 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of Version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include "nodice/board.h"

#include <iostream>
#include "nodice/config.h"
#include "nodice/object.h"

namespace
{
  static const float swap_factor = 10.0f;
  static const float swap_step = 0.5f;
} // anonymous namespace


NoDice::Board::
Board(NoDice::Config const* config)
: config_(config)
, state_(state_idle)
{
  for (int y = 0; y < config_->board_size(); ++y)
  {
    for (int x = 0; x < config_->board_size(); ++x)
    {
      objects_.push_back(ObjectPtr(new Object(NoDice::chooseAShape(),
                                     Vector3f(x * 2.0f, y * 2.0f, 0.0f))));
    }
  }
  if (find_wins().size() > 0)
  {
    state_ = state_removing;
  }
}


NoDice::ObjectPtr&  NoDice::Board::
at(const NoDice::Vector2i& p)
{ return objects_[p.x + p.y * config_->board_size()]; }


const NoDice::ObjectPtr&  NoDice::Board::
at(int x, int y) const
{ return objects_[x + y * config_->board_size()]; }


void NoDice::Board::
update()
{
  for (auto it = objects_.begin(); it != objects_.end(); ++it)
  {
    (*it)->update();
  }

  switch (state_)
  {
    case state_swapping:
    {
      swap_step_ += swap_step;
      if (swap_step_ > swap_factor)
      {
        at(swap_obj_[0]).swap(at(swap_obj_[1]));
        for (auto it = objects_.begin(); it != objects_.end(); ++it)
        {
          (*it)->setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
        }
        state_ = state_idle;
      }
      break;
    }

    case state_removing:
    {
      // Wait until all disappearing is finished.
      for (auto it = removal_queue_.begin(); it != removal_queue_.end(); ++it)
      {
        if (!at(*it)->hasDisappeared())
          return;
      }

      state_ = state_falling;
      for (int x = 0; x < config_->board_size(); ++x)
      {
        int drop = 0;
        for (int y = 0; y < config_->board_size(); ++y)
        {
          if (at(x, y)->hasDisappeared())
          {
            ++drop;
          }
          else if (drop > 0)
          {
            at(x, y)->startFalling(Vector3f(2.0f * x, 2.0f * (y - drop), 0.0f));
            falling_queue_.push_back(std::make_pair(Vector2i(x, y),
                                                    Vector2i(x, y - drop)));
          }
        }
        for (int y = 0; y < drop; ++y)
        {
          int new_y = config_->board_size() - y - 1;
          create_queue_.push_back(Vector2i(x, new_y));
        }
      }
      removal_queue_.clear();
      break;
    }

    case state_falling:
    {
      // Wait until all falling is finished.
      for (auto it = falling_queue_.begin(); it != falling_queue_.end(); ++it)
      {
        if (at((*it).first)->isFalling())
          return;
      }

      // Percolate removed jobbies out
      for (auto it = falling_queue_.begin(); it != falling_queue_.end(); ++it)
      {
        at((*it).first).swap(at((*it).second));
        ObjectPtr().swap(at((*it).first));
      }
      falling_queue_.clear();

      // Fill in the blanks.
      for (auto it = create_queue_.begin(); it != create_queue_.end(); ++it)
      {
        at(*it) = ObjectPtr(new Object(NoDice::chooseAShape(),
                            Vector3f((*it).x * 2.0f, (*it).y * 2.0f, 0.0f)));
      }
      create_queue_.clear();

      state_ = state_idle;
      break;
    }

    default:
      break;
  }
}


void NoDice::Board::
draw() const
{
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  for (int y = 0; y < config_->board_size(); ++y)
  {
    for (int x = 0; x < config_->board_size(); ++x)
    {
      at(x, y)->draw();
    }
  }
  glPopMatrix();
}


void NoDice::Board::
start_swap(NoDice::Vector2i pos1, NoDice::Vector2i pos2)
{
  swap_obj_[0] = pos1;
  swap_obj_[1] = pos2;
  ObjectPtr obj1 = at(pos1.x, pos1.y);
  ObjectPtr obj2 = at(pos2.x, pos2.y);
  obj1->setVelocity(Vector3f(float(pos2.x-pos1.x) / swap_factor,
                             float(pos2.y-pos1.y) / swap_factor,
                             0.0f));
  obj2->setVelocity(Vector3f(float(pos1.x-pos2.x) / swap_factor,
                             float(pos1.y-pos2.y) / swap_factor,
                             0.0f));
  swap_step_ = 0.0f;
  state_ = state_swapping;
}


void NoDice::Board::
un_swap()
{
  start_swap(swap_obj_[1], swap_obj_[0]);
}


bool NoDice::Board::
is_swapping() const
{
  return state_ == state_swapping;
}


bool NoDice::Board::
is_replacing() const
{
  return state_ == state_removing || state_ == state_falling;
}


/**
 * Looks for 3 (or more) matching objects in a row, horizontal or vertical.
 */
NoDice::ObjectBrace NoDice::Board::
find_wins()
{
  ObjectBrace matches;

  // check for 3-or-more-in-a-row
  for (int y = 0; y < config_->board_size(); ++y)
  {
    int x = 0;
    while (x <  config_->board_size()-2)
    {
      int count = 0;
      const ObjectPtr& obj = at(x, y);
      auto type = obj->type();
      for (int i = x; i < config_->board_size(); ++i)
      {
        if (type == at(i, y)->type())
        {
          ++count;
        }
        else
          break;
      }
      if (count > 2)
      {
        ObjectBag brace;
        for (int i = 0; i < count; ++i)
        {
          const Vector2i p(x+i, y);
          removal_queue_.push_back(p);
          ObjectPtr& o = at(p);
          brace.push_back(o);
          o->startDisappearing();
        }
        matches.push_back(brace);
        x += count;
      }
      else
      {
        ++x;
      }
    }
  }

  // check for 3-in-a-column
  for (int x = 0; x < config_->board_size(); ++x)
  {
    int y = 0;
    while (y <  config_->board_size()-2)
    {
      int count = 0;
      const ObjectPtr& obj = at(x, y);
      auto type = obj->type();
      for (int i = y; i < config_->board_size(); ++i)
      {
        if (type == at(x, i)->type())
        {
          ++count;
        }
        else
          break;
      }
      if (count > 2)
      {
        ObjectBag brace;
        for (int i = 0; i < count; ++i)
        {
          const Vector2i p(x, y+i);
          removal_queue_.push_back(p);
          ObjectPtr& o = at(p);
          brace.push_back(o);
          o->startDisappearing();
        }
        matches.push_back(brace);
        y += count;
      }
      else
      {
        ++y;
      }
    }
  }

  if (matches.size() > 0)
    state_ = state_removing;

  return matches;
}
