/**
 * @file nodice/videocontextsdl.cpp
 * @brief Implemntation of the nodice/videocontextsdl module.
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
#include "nodice/videocontextsdl.h"

#include <iostream>
#include "nodice/config.h"


NoDice::VideoContextSDL::
VideoContextSDL(Config const*)
{
  if (0 != ::SDL_InitSubSystem(SDL_INIT_VIDEO))
  {
    std::cerr << "*** ERRROR in SDL_InitSubSystem(SDL_INIT_VIDEO): "
              << ::SDL_GetError() << "\n";
    exit(1);
  }

  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  window_ = SDL_CreateWindow("No Dice",
                             SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                             640, 480,
                             SDL_WINDOW_OPENGL);

  SDL_GLContext context = SDL_GL_CreateContext(window_);
  if (!context)
  {
    std::cerr << "*** ERRROR in SDL_GL_CreateContext(): " << ::SDL_GetError() << "\n";
    exit(1);
  }
}

void NoDice::VideoContextSDL::swapBuffers()
{
  SDL_GL_SwapWindow(window_);
}


