/**
 * @file nodice/gamestate.h
 * @brief Public interface of the nodice/gamestate module.
 *
 * Copyright 2010 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
#ifndef NODICE_GAMESTATE_H
#define NODICE_GAMESTATE_H 1

#include "nodice/config.h"
#include <SDL/SDL_events.h>
#include <tr1/memory>


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
		GameState(Config& config);
		virtual ~GameState() = 0;

		virtual void pause();
		virtual void resume();

		virtual void key(SDL_keysym keysym);
		virtual void pointerMove(int x, int y, int dx, int dy);
		virtual void pointerClick(int x, int y, PointerAction action);
		virtual void update(App& app) = 0;

		virtual void draw(Video& video) = 0;

	protected:
		Config& m_config;
	};

	typedef std::tr1::shared_ptr<GameState> GameStatePtr;

} // namespace NoDice

#endif // NODICE_GAMESTATE_H
