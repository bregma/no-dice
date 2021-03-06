/**
 * @file nodice/gamestate.h
 * @brief Public interface of the nodice/gamestate module.
 */
/*
 * Copyright 2010,2017 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
#ifndef NODICE_GAMESTATE_H
#define NODICE_GAMESTATE_H 1

#include <SDL_events.h>
#include <memory>


namespace NoDice
{
  class App;
  class Video;

  /**
   * An abstract base class for things implementing a stateful part of the game.
   */
  class GameState
  {
  public:
    enum PointerAction
    {
      pointerUp,
      pointerDown,
      pointerDoubleDown
    };

  public:
    GameState(App* app);
    virtual ~GameState() = 0;

    virtual void pause();
    virtual void resume();

    virtual void key(SDL_Keysym keysym);
    virtual void pointerMove(int x, int y, int dx, int dy);
    virtual void pointerClick(int x, int y, PointerAction action);
    virtual void update(App& app) = 0;

    virtual void draw(Video& video) = 0;

  protected:
    App*  app_;
  };

  typedef std::shared_ptr<GameState> GameStatePtr;

} // namespace NoDice

#endif // NODICE_GAMESTATE_H
