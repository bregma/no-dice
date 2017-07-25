/**
 * @file nodice/playstate.cpp
 * @brief Implemntation of the nodice/playstate module.
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
#include "nodice_config.h"
#include "nodice/playstate.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include "nodice/colour.h"
#include "nodice/font.h"
#include "nodice/object.h"
#include "nodice/shape.h"
#include "nodice/video.h"

#define SCORE_FONT "FreeSans"


namespace
{
  static const GLfloat near =  0.0f;
  static const GLfloat far  = 10.0f;
  static const NoDice::Vector3f board_scale(1.0f/12.0f, 1.0f/12.0f, 1.0f/12.0f);
  static const NoDice::Vector3f board_pos(-0.3f, -0.5f, -1.0f);
  static const NoDice::Vector4f lightAmbient(0.2f, 0.2f, 0.2f, 1.0f);
  static const NoDice::Vector4f lightDiffuse(0.8f, 0.8f, 0.8f, 1.0f);
  static const NoDice::Vector4f lightPosition(2.0f, 2.0f, 3.0f, 0.0f);
  static const NoDice::Vector3f lightDirection(-2.0f, -2.0f, -3.0f);
  static const int mouseMoveThreshold = 20;
} // anonymous namespace


NoDice::PlayState::
PlayState(Config const* config)
: GameState(config)
, state_(state_idle)
, gameboard_(config)
, score_font_(getFont(SCORE_FONT, config->screen_height() / 18))
, mouse_is_down_(false)
, multiplier_(0)
, score_(0)
{
  // generate unproject matrix
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  int w = config->screen_width();
  int h = config->screen_height();
  float right = 1.0f;
  float top   = 1.0f;
  if (h < w)
    top = float(h) / float(w);
  else
    right = float(w) / float(h);
#ifdef HAVE_OPENGL_ES
  glFrustumf(-right, right, -top, top, near, far);
#else
  glOrtho(-right, right, -top, top, near, far);
#endif
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glTranslatef(board_pos.x, board_pos.y, board_pos.z);
  glScalef(board_scale.x, board_scale.y, board_scale.z);
  Matrix4f projection;
  glGetFloatv(GL_PROJECTION_MATRIX, projection.array);
  Matrix4f modelview;
  glGetFloatv(GL_MODELVIEW_MATRIX, modelview.array);
  (projection * modelview).getInverse(unproject_);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
}


NoDice::PlayState::
~PlayState()
{
}


void NoDice::PlayState::
pointerMove(int x, int y, int dx, int dy)
{
  if (mouse_is_down_ && state_ != state_swapping)
  {
    Vector2i d = mouse_down_pos_ - Vector2i(x, y);
    if (d.lengthSquared() < mouseMoveThreshold * mouseMoveThreshold)
      return;

    Vector2i pos2 = selected_pos_;
    if (std::abs(dx) > std::abs(dy))
    {
      if (dx < 0 && selected_pos_.x > 0)
          --pos2.x;
      else if (selected_pos_.x < config_->board_size()-1)
          ++pos2.x;
      else
        return;
    }
    else
    {
      if (dy > 0 && selected_pos_.y > 0)
          --pos2.y;
      else if (selected_pos_.y < config_->board_size()-1)
          ++pos2.y;
      else
        return;
    }
    state_ = state_swapping;
    gameboard_.start_swap(selected_pos_, pos2);
  }
}


void NoDice::PlayState::
pointerClick(int x, int y, PointerAction action)
{
  if (action == pointerUp)
  {
    mouse_is_down_ = false;
    mouse_down_pos_.set(0, 0);
  }
  else if (action == pointerDown)
  {
    mouse_down_pos_.set(x, y);
    float win_width = float(config_->screen_width()) / 2.0f;
    float win_height = float(config_->screen_height()) / 2.0f;
    float unit_x = float(x - win_width) / win_width;
    float unit_y = -float(y - win_height) / win_height;

    Vector4f ray(unit_x, unit_y, 0.0f, 1.0f);
    Vector4f beam = unproject_ * ray;
    selected_pos_.x = int(beam.x / 2.0f + 0.50f);
    selected_pos_.y = int(beam.y / 2.0f + 0.50f);
    if (selected_pos_.x >= config_->board_size() || selected_pos_.x < 0)
      return;
    if (selected_pos_.y >= config_->board_size() || selected_pos_.y < 0)
      return;

    ObjectPtr obj = gameboard_.at(selected_pos_.x, selected_pos_.y);
    obj->setHighlight(true);

    mouse_is_down_ = true;
  }
}


void NoDice::PlayState::
calculateScore(const ObjectBrace& matches)
{
  for (auto it = matches.begin(); it != matches.end(); ++it)
  {
    int match_score = multiplier_;
    std::ostringstream ostr;
    ostr << it->size() << it->at(0)->type();
    if (multiplier_)
    {
      ostr << "+" << multiplier_;
    }
    std::cerr << ostr.str() << "(";
    for (auto obj = it->begin(); obj != it->end(); ++obj)
    {
      int score = (*obj)->score();
      std::cerr << " " << score;
      match_score += score;
    }
    std::cerr << " ) total=" << match_score << "\n";
    score_ += match_score;
    win_messages_.push_back(ostr.str());
  }
  state_ = state_replacing;
  ++multiplier_;
}


void NoDice::PlayState::
update(App& app NODICE_UNUSED)
{
  gameboard_.update();
  switch (state_)
  {
    case state_swapping:
    {
      if (!gameboard_.is_swapping())
      {
        ObjectBrace matches(gameboard_.find_wins());
        if (matches.size())
        {
          calculateScore(matches);
        }
        else
        {
          gameboard_.un_swap();
          state_ = state_unswapping;
          multiplier_ = 0;
        }
      }
      break;
    }

    case state_unswapping:
    {
      if (!gameboard_.is_swapping())
      {
        state_ = state_idle; // temp. for now
      }
      break;
    }

    case state_replacing:
    {
      if (!gameboard_.is_replacing())
      {
        ObjectBrace matches(gameboard_.find_wins());
        if (matches.size() > 0)
        {
          calculateScore(matches);
        }
        else
        {
          std::cerr << __FUNCTION__ << " has no wins\n";
          state_ = state_idle;
          multiplier_ = 0;
          win_messages_.clear();
        }
      }
      break;
    }

                default:
                  break;
  }
}


void NoDice::PlayState::
draw(Video& video NODICE_UNUSED) 
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  // Adjust projection to take aspect ratio into account.
  int w = config_->screen_width();
  int h = config_->screen_height();
  float right = 1.0f;
  float top   = 1.0f;
  if (h < w)
    top = float(h) / float(w);
  else
    right = float(w) / float(h);
#ifdef HAVE_OPENGL_ES
  glFrustumf(-right, right, -top, top, near, far);
#else
  glOrtho(-right, right, -top, top, near, far);
#endif
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_CULL_FACE);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_NORMALIZE);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);

  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white.rgba);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 60.0f);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient.rgba);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse.rgba);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white.rgba);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition.xyzw);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection.xyz);
  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 1.2f);
  glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 20.0f);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glTranslatef(board_pos.x, board_pos.y, board_pos.z);
  glScalef(board_scale.x, board_scale.y, board_scale.z);
  gameboard_.draw();
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  float y = 300.0f;
  std::ostringstream ostr;
  ostr << std::setw(5) << std::setfill('0') << score_;
  glColor4fv(white.rgba);
  score_font_.print(10.0f, y, 1.0f, ostr.str());

  for (auto it = win_messages_.begin(); it != win_messages_.end(); ++it)
  {
    y -= 30;
    score_font_.print(10.0f, y, 0.8f, *it);
  }
  check_gl_error("playstate END");
}


