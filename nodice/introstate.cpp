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
#include "nodice/font.h"
#include "nodice/playstate.h"
#include "nodice/video.h"

#define MENU_FONT "spindle"


namespace 
{
  struct MenuEntry
  {
    const char*                    title;
    vmml::Vector2f                 pos;
    NoDice::IntroState::NextState  nextState;
  };

  static MenuEntry entry[] = 
  {
    { "options", vmml::Vector2f(0.0f, 0.0f), NoDice::IntroState::next_state_options },
    { "play",    vmml::Vector2f(0.0f, 0.0f), NoDice::IntroState::next_state_play },
    { "quit",    vmml::Vector2f(0.0f, 0.0f), NoDice::IntroState::next_state_quit }
  };

  static const std::size_t menuCount = sizeof(entry) / sizeof(MenuEntry);

  static float titleColour[] = { 1.00f, 0.10f, 0.10f, 1.00f };
  static float selectedColour[] = { 0.80f, 0.50f, 1.00f, 0.80f };
  static float unselectedColour[] = { 0.20f, 0.20f, 0.80f, 0.80f };

} // anonymous namespace


NoDice::IntroState::
IntroState(Config const* config,
           Video const&  video NODICE_UNUSED)
: GameState(config)
, is_active_(true)
, menu_font_(getFont(MENU_FONT, config->screen_height() / 18))
, title_pos_(0.25 * config->screen_width(), 0.75 * config->screen_height())
, selected_(0)
, next_state_(next_state_same)
{
  const float vspacing = -2.0f * menu_font_.height();
  entry[0].pos.set(title_pos_.x, title_pos_.y + vspacing);
  for (std::size_t i = 1; i < menuCount; ++i)
  {
    entry[i].pos.set(entry[i-1].pos.x, entry[i-1].pos.y + vspacing);
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
key(SDL_keysym keysym)
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
      if (std::size_t(selected_) >= menuCount)
        selected_ = menuCount-1;
      break;

    case SDLK_RETURN:
    case SDLK_KP_ENTER:
      next_state_ = entry[selected_].nextState;
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
      app.popGameState();
      break;

    case next_state_play:
      app.pushGameState(GameStatePtr(new PlayState(config_)));
      break;

    default:
      break;
  }
}


void NoDice::IntroState::
draw(Video& video NODICE_UNUSED)
{
  glColor4fv(titleColour);
  menu_font_.print(title_pos_.x, title_pos_.y, 1.0f, "No Dice!");

  for (std::size_t i = 0; i < menuCount; ++i)
  {
    if (i == std::size_t(selected_))
      glColor4fv(selectedColour);
    else
      glColor4fv(unselectedColour);
    menu_font_.print(entry[i].pos.x, entry[i].pos.y, 1.0f, entry[i].title);
  }
}


