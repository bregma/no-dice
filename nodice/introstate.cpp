/**
 * @file introstate.cpp
 * @brief Implemntation of the introstate module.
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
#include "nodice_config.h"
#include "nodice/introstate.h"

#include "nodice/app.h"
#include "nodice/colour.h"
#include "nodice/config.h"
#include "nodice/font.h"
#include "nodice/playstate.h"
#include "nodice/shader.h"
#include "nodice/shaderprogram.h"
#include "nodice/video.h"

#define MENU_FONT "spindle"


namespace 
{
  struct MenuEntry
  {
    const char*                    title;
    NoDice::vec2                   pos;
    NoDice::IntroState::NextState  nextState;
  };

  std::vector<MenuEntry> menu_entry{
    { "options", NoDice::vec2{0.0f, 0.0f}, NoDice::IntroState::next_state_options },
    { "play",    NoDice::vec2{0.0f, 0.0f}, NoDice::IntroState::next_state_play },
    { "quit",    NoDice::vec2{0.0f, 0.0f}, NoDice::IntroState::next_state_quit }
  };

  NoDice::Colour titleColour{ 1.00f, 0.10f, 0.10f, 1.00f };
  NoDice::Colour selectedColour{ 0.80f, 0.50f, 1.00f, 0.80f };
  NoDice::Colour unselectedColour{ 0.20f, 0.20f, 0.80f, 0.80f };

} // anonymous namespace


NoDice::IntroState::
IntroState(NoDice::App*  app,
           Video const&  video NODICE_UNUSED)
: GameState(app)
, is_active_(true)
, menu_font_(app_->font_cache().get_font(MENU_FONT, app_->config().screen_height() / 18))
, title_pos_(0.25 * app_->config().screen_width(), 0.75 * app_->config().screen_height())
, selected_(0)
, next_state_(next_state_same)
{
  const float vspacing = -2.0f * menu_font_.height();
  menu_entry[0].pos.set(title_pos_.x, title_pos_.y + vspacing);
  for (std::size_t i = 1; i < menu_entry.size(); ++i)
  {
    menu_entry[i].pos.set(menu_entry[i-1].pos.x, menu_entry[i-1].pos.y + vspacing);
  }
}

NoDice::IntroState::
~IntroState()
{
}

void NoDice::IntroState::
pause()
{
  is_active_ = false;
}


void NoDice::IntroState::
resume()
{
  is_active_ = true;
}


void NoDice::IntroState::
key(SDL_Keysym keysym)
{
  switch (keysym.sym)
  {
    case SDLK_UP:
      --selected_;
      if (selected_ < 0)
        selected_ = 0;
      break;

    case SDLK_DOWN:
      ++selected_;
      if (std::size_t(selected_) >= menu_entry.size())
        selected_ = menu_entry.size()-1;
      break;

    case SDLK_RETURN:
    case SDLK_KP_ENTER:
      next_state_ = menu_entry[selected_].nextState;
      break;

    case SDLK_o:
      next_state_ = next_state_options;
      break;

    case SDLK_p:
      next_state_ = next_state_play;
      break;

    case SDLK_q:
      next_state_ = next_state_quit;
      break;

          default:
                  break;
  }
}


void NoDice::IntroState::
pointerMove(int x NODICE_UNUSED,
            int y NODICE_UNUSED,
            int dx NODICE_UNUSED, 
            int dy NODICE_UNUSED)
{
}


void NoDice::IntroState::
pointerClick(int x NODICE_UNUSED,
             int y NODICE_UNUSED,
             PointerAction action NODICE_UNUSED)
{
}


void NoDice::IntroState::
update(App& app)
{
  switch (next_state_)
  {
    case next_state_quit:
      app.pop_game_state();
      break;

    case next_state_play:
      app.push_game_state(GameStatePtr(new PlayState(app_)));
      break;

    default:
      break;
  }
}


void NoDice::IntroState::
draw(Video& video NODICE_UNUSED)
{
#ifdef USE_FIXED_PIPELINE
  glColor4fv(titleColour.rgba);
  check_gl_error("IntroState::draw() glColor4fv(1a)");
#else
  Shader vertex_shader(app_->config(), GL_VERTEX_SHADER, "intro-vertex.glsl");
  Shader fragment_shader(app_->config(), GL_FRAGMENT_SHADER, "intro-fragment.glsl");
  ShaderProgram shader_program;
  shader_program.attach(vertex_shader);
  shader_program.attach(fragment_shader);
  shader_program.activate();
  check_gl_error("IntroState::draw() shader_program.activate()");
#endif 

  menu_font_.print(title_pos_.x, title_pos_.y, 1.0f, "No Dice!");

  for (std::size_t i = 0; i < menu_entry.size(); ++i)
  {
#ifdef USE_FIXED_PIPELINE
    if (i == std::size_t(selected_))
    {
      glColor4fv(selectedColour.rgba);
      check_gl_error("IntroState::draw() glColor4fv(2)");
    }
    else
    {
      glColor4fv(unselectedColour.rgba);
      check_gl_error("IntroState::draw() glColor4fv(3)");
    }
#endif
    menu_font_.print(menu_entry[i].pos.x, menu_entry[i].pos.y, 1.0f, menu_entry[i].title);
  }
#ifndef USE_FIXED_PIPELINE
  shader_program.deactivate();
#endif
}


