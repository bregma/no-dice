/**
 * @file introstate.cpp
 * @brief Implemntation of the introstate module.
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
IntroState(Config& config,
           const Video& video NODICE_UNUSED)
: GameState(config)
, m_isActive(true)
, m_menuFont(getFont(MENU_FONT, config.screenHeight() / 18))
, m_titlePos(0.25 * config.screenWidth(), 0.75 * config.screenHeight())
, m_selected(0)
, m_nextState(next_state_same)
{
	const float vspacing = -2.0f * m_menuFont.height();
	entry[0].pos.set(m_titlePos.x, m_titlePos.y + vspacing);
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
	m_isActive = false;
}


void NoDice::IntroState::
resume()
{
	m_isActive = true;
}


void NoDice::IntroState::
key(SDL_keysym keysym)
{
	switch (keysym.sym)
	{
		case SDLK_UP:
			--m_selected;
			if (m_selected < 0)
				m_selected = 0;
			break;

		case SDLK_DOWN:
			++m_selected;
			if (std::size_t(m_selected) >= menuCount)
				m_selected = menuCount-1;
			break;

		case SDLK_RETURN:
		case SDLK_KP_ENTER:
			m_nextState = entry[m_selected].nextState;
			break;

		case SDLK_o:
			m_nextState = next_state_options;
			break;

		case SDLK_p:
			m_nextState = next_state_play;
			break;

		case SDLK_q:
			m_nextState = next_state_quit;
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
	switch (m_nextState)
	{
		case next_state_quit:
			app.popGameState();
			break;

		case next_state_play:
			app.pushGameState(GameStatePtr(new PlayState(m_config)));
			break;

		default:
			break;
	}
}


void NoDice::IntroState::
draw(Video& video NODICE_UNUSED)
{
	glColor4fv(titleColour);
	m_menuFont.print(m_titlePos.x, m_titlePos.y, 1.0f, "No Dice!");

	for (std::size_t i = 0; i < menuCount; ++i)
	{
		if (i == std::size_t(m_selected))
			glColor4fv(selectedColour);
		else
			glColor4fv(unselectedColour);
		m_menuFont.print(entry[i].pos.x, entry[i].pos.y, 1.0f, entry[i].title);
	}
}


