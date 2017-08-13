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
#include <SDL.h>


namespace
{
  static const Uint32 UPDATE_TICKS         = 1000/44;
  static const Uint32 ACTIVE_FRAME_DELAY   = 10;
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
, shader_cache_(this)
, font_cache_(this)
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


NoDice::ShaderCache& NoDice::App::
shader_cache()
{
  return shader_cache_;
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


namespace
{

std::string
event_to_name(SDL_EventType type)
{
  static struct {
    SDL_EventType type;
    std::string   name;
  } sdl_event_types[] = {
    { SDL_FIRSTEVENT,               "SDL_FIRSTEVENT" },
    { SDL_QUIT,                     "SDL_QUIT" },
    { SDL_APP_TERMINATING,          "SDL_APP_TERMINATING" },
    { SDL_APP_LOWMEMORY,            "SDL_APP_LOWMEMORY" },
    { SDL_APP_WILLENTERBACKGROUND,  "SDL_APP_WILLENTERBACKGROUND" },
    { SDL_APP_DIDENTERBACKGROUND,   "SDL_APP_DIDENTERBACKGROUND" },
    { SDL_APP_WILLENTERFOREGROUND,  "SDL_APP_WILLENTERFOREGROUND" },
    { SDL_APP_DIDENTERFOREGROUND,   "SDL_APP_DIDENTERFOREGROUND" },
    { SDL_WINDOWEVENT,              "SDL_WINDOWEVENT" },
    { SDL_SYSWMEVENT,               "SDL_SYSWMEVENT" },
    { SDL_KEYDOWN,                  "SDL_KEYDOWN" },
    { SDL_KEYUP,                    "SDL_KEYUP" },
    { SDL_TEXTEDITING,              "SDL_TEXTEDITING" },
    { SDL_TEXTINPUT,                "SDL_TEXTINPUT" },
#if SDL_VERSION_ATLEAST(2, 0, 4)
    { SDL_KEYMAPCHANGED,            "SDL_KEYMAPCHANGED" },
#endif
    { SDL_MOUSEMOTION,              "SDL_MOUSEMOTION" },
    { SDL_MOUSEBUTTONDOWN,          "SDL_MOUSEBUTTONDOWN" },
    { SDL_MOUSEBUTTONUP,            "SDL_MOUSEBUTTONUP" },
    { SDL_MOUSEWHEEL,               "SDL_MOUSEWHEEL" },
    { SDL_JOYAXISMOTION,            "SDL_JOYAXISMOTION" },
    { SDL_JOYBALLMOTION,            "SDL_JOYBALLMOTION" },
    { SDL_JOYHATMOTION,             "SDL_JOYHATMOTION" },
    { SDL_JOYBUTTONDOWN,            "SDL_JOYBUTTONDOWN" },
    { SDL_JOYBUTTONUP,              "SDL_JOYBUTTONUP" },
    { SDL_JOYDEVICEADDED,           "SDL_JOYDEVICEADDED" },
    { SDL_JOYDEVICEREMOVED,         "SDL_JOYDEVICEREMOVED" },
    { SDL_CONTROLLERAXISMOTION,     "SDL_CONTROLLERAXISMOTION" },
    { SDL_CONTROLLERBUTTONDOWN,     "SDL_CONTROLLERBUTTONDOWN" },
    { SDL_CONTROLLERBUTTONUP,       "SDL_CONTROLLERBUTTONUP" },
    { SDL_CONTROLLERDEVICEADDED,    "SDL_CONTROLLERDEVICEADDED" },
    { SDL_CONTROLLERDEVICEREMOVED,  "SDL_CONTROLLERDEVICEREMOVED" },
    { SDL_CONTROLLERDEVICEREMAPPED, "SDL_CONTROLLERDEVICEREMAPPED" },
    { SDL_FINGERDOWN,               "SDL_FINGERDOWN" },
    { SDL_FINGERUP,                 "SDL_FINGERUP" },
    { SDL_FINGERMOTION,             "SDL_FINGERMOTION" },
    { SDL_DOLLARGESTURE,            "SDL_DOLLARGESTURE" },
    { SDL_DOLLARRECORD,             "SDL_DOLLARRECORD" },
    { SDL_MULTIGESTURE,             "SDL_MULTIGESTURE" },
    { SDL_CLIPBOARDUPDATE,          "SDL_CLIPBOARDUPDATE" },
    { SDL_DROPFILE,                 "SDL_DROPFILE" },
#if SDL_VERSION_ATLEAST(2, 0, 4)
    { SDL_AUDIODEVICEADDED,         "SDL_AUDIODEVICEADDED" },
    { SDL_AUDIODEVICEREMOVED,       "SDL_AUDIODEVICEREMOVED" },
#endif
    { SDL_RENDER_TARGETS_RESET,     "SDL_RENDER_TARGETS_RESET" },
#if SDL_VERSION_ATLEAST(2, 0, 4)
    { SDL_RENDER_DEVICE_RESET,      "SDL_RENDER_DEVICE_RESET" },
#endif
  };

  for (auto const& event: sdl_event_types)
  {
    if (type == event.type)
    {
      return event.name;
    }
  }
  return "unknown";
}

} // anonymous namespace


void NoDice::App::
process_input()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    if (config_->is_debug_mode())
      std::cerr << "==smw> event type " << event_to_name(static_cast<SDL_EventType>(event.type)) << " received.\n";

    switch (event.type)
    {
#if 0
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
#endif

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
                  << " keysym.scancode=" << event.key.keysym.scancode
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

