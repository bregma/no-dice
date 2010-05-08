/**
 * @file introstate.h
 * @brief Public interface of the introstate module.
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
#ifndef IntroState_H
#define IntroState_H 1

#include "nodice/gamestate.h"
#include "vmmlib/vmmlib.h"


namespace NoDice
{
	class Font;

	/**
	 * Provides the "game introduction" state:  mostly just the loading screen.
	 */
	class IntroState
	: public GameState
	{
	public:
		IntroState(const Video& video);

		~IntroState();

		void pause();
		void resume();

		void key(SDL_keysym keysym);
		void pointerMove(int x, int y, int dx, int dy);
		void pointerClick(int x, int y, PointerAction action);
		void update(App& app);

		void draw(Video& video);

	private:
		bool            m_isActive;
		Font&           m_menuFont;
		vmml::Vector2f  m_titlePos;
		vmml::Vector2f  m_optionsPos;
		vmml::Vector2f  m_playPos;
		vmml::Vector2f  m_quitPos;
	};

} // namespace NoDice

#endif // IntroState_H
