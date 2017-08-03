/**
 * @file nodice/videocontextsdl.h
 * @brief Public interface of the nodice/videocontextsdl module.
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
#ifndef NODICE_VIDEOCONTEXTSDL_H
#define NODICE_VIDEOCONTEXTSDL_H 1

#include "nodice/videocontext.h"
#include <SDL.h>


namespace NoDice
{
  class Config;


  class VideoContextSDL
  : public VideoContext
  {
  public:
    VideoContextSDL(Config const* config);

    void
    swapBuffers() override;

  private:
    SDL_Window* window_;
  };

} // namespace NoDice

#endif // NODICE_VIDEOCONTEXTSDL_H
