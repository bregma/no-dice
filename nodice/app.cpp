/**
 * @file nodice/app.cpp
 * @brief Implemntation of the no-dice app module.
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
#include "nodice/app.h"

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "nodice/config.h"
#include "nodice/introstate.h"
#include "nodice/video.h"
#include <SDL/SDL.h>


namespace
{
  static const Uint32 UPDATE_TICKS         = 1000/44;
  static const Uint32 ACTIVE_FRAME_DELAY   = 10;
  static const Uint32 INACTIVE_FRAME_DELAY = 300;
} // anonymous namespace


NoDice::App::SdlInit::
SdlInit()
{
  Uint32 flags = SDL_INIT_NOPARACHUTE;
  int istat = ::SDL_Init(flags);
  if (istat < 0)
  {
    std::cerr << "*** ERRROR in SDL_Init: " << ::SDL_GetError() << "\n";
    exit(1);
  }

  std::atexit(::SDL_Quit);
}


NoDice::App::SdlInit::
~SdlInit()
{
  ::SDL_Quit();
}

/**
 * The App object takes care of initializing the even system.
 */
NoDice::App::
App(NoDice::Config* config)
: config_(config)
, sdl_init_()
, video_(config)
, font_cache_(config)
, game_is_running_(false)
{
  std::srand(std::time(NULL));
  push_game_state(GameStatePtr(new IntroState(this, video_)));
}


NoDice::App::
~App()
{
}

int NoDice::App::
run()
{
  game_is_running_ = true;
  Uint32 epochTics = SDL_GetTicks();
  while (game_is_running_)
  {
    Uint32 currentTics = SDL_GetTicks();
    Uint32 deltaTics = currentTics - epochTics;
    if (deltaTics > UPDATE_TICKS)
    {
      update();
      epochTics = currentTics;
    }
    state_stack_.top()->draw(video_);
    video_.update();

    SDL_Delay(ACTIVE_FRAME_DELAY);
  }
  return 0;
}


void NoDice::App::
push_game_state(GameStatePtr state)
{
  state_stack_.push(state);
}


void NoDice::App::
pop_game_state()
{
  state_stack_.pop();
}


NoDice::Config& NoDice::App::
config()
{
  return *config_;
}


NoDice::FontCache& NoDice::App::
font_cache()
{
  return font_cache_;
}


void NoDice::App::
stop_game()
{
  game_is_running_ = false;
}


void NoDice::App::
process_input()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {

    switch (event.type)
    {
    case SDL_ACTIVEEVENT:
      if (event.active.gain != 0)
      {
        state_stack_.top()->resume();
      }
      else
      {
        state_stack_.top()->pause();
      }
      break;

    case SDL_MOUSEMOTION:
      state_stack_.top()->pointerMove(event.motion.x, event.motion.y,
                                      event.motion.xrel, event.motion.yrel);
      break;

    case SDL_MOUSEBUTTONDOWN:
      if (event.button.button == SDL_BUTTON_LEFT)
        state_stack_.top()->pointerClick(event.button.x, event.button.y,
                                        GameState::pointerDown);
      break;

    case SDL_MOUSEBUTTONUP:
      if (event.button.button == SDL_BUTTON_LEFT)
        state_stack_.top()->pointerClick(event.button.x, event.button.y,
                                        GameState::pointerUp);
      break;

    case SDL_KEYDOWN:
      if (config_->is_debug_mode())
        std::cerr << "==smw> SDL_KEYDOWN event type received:"
                  << " keysym.scancode=" << (int)event.key.keysym.scancode
                  << " keysym.sym=" << event.key.keysym.sym
                  << " keysym.mod=" << event.key.keysym.mod
                  << " keysym.unicode=" << event.key.keysym.unicode
                  << "\n";
      if (event.key.keysym.sym == 27) // esc
        stop_game();
      else
        state_stack_.top()->key(event.key.keysym);
      break;

    case SDL_QUIT:
      if (config_->is_debug_mode())
        std::cerr << "==smw> SDL_QUIT event type received.\n";
      stop_game();
      break;

    default:
      if (config_->is_debug_mode())
        std::cerr << "==smw> event type " << static_cast<int>(event.type) << " received.\n";
      break;
    }
  }
}


void NoDice::App::
update()
{
  process_input();
  state_stack_.top()->update(*this);
  if (state_stack_.empty())
  {
    stop_game();
  }
}

