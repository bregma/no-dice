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
#include "introstate.h"

#include "nodice/font.h"
#include "nodice/video.h"

namespace 
{
	struct MenuEntry
	{
		const char*    title;
		vmml::Vector2f pos;
	};

	static MenuEntry entry[] = 
	{
		{ "options", vmml::Vector2f(0.0f, 0.0f) },
		{ "play",    vmml::Vector2f(0.0f, 0.0f) },
		{ "quit",    vmml::Vector2f(0.0f, 0.0f) }
	};

	static const std::size_t menuCount = sizeof(entry) / sizeof(MenuEntry);

	static float titleColour[] = { 1.00f, 0.10f, 0.10f, 1.00f };
	static float selectedColour[] = { 0.50f, 0.50f, 1.00f, 0.80f };
	static float unselectedColour[] = { 0.20f, 0.20f, 0.80f, 0.80f };

} // anonymous namespace


NoDice::IntroState::
IntroState(const Video& video)
: m_isActive(true)
, m_menuFont(getFont("spindle", video.screenHeight() / 18))
, m_titlePos(0.25 * video.screenWidth(), 0.75 * video.screenHeight())
, m_selected(0)
{
	const float vspacing = -2.0f * m_menuFont.height();
	entry[0].pos.set(m_titlePos.x, m_titlePos.y + vspacing);
	for (int i = 1; i < menuCount; ++i)
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
	if (keysym.sym == SDLK_UP)
	{
		--m_selected;
		if (m_selected < 0)
			m_selected = 0;
	}
	else if (keysym.sym == SDLK_DOWN)
	{
		++m_selected;
		if (m_selected >= menuCount)
			m_selected = menuCount-1;
	}
}


void NoDice::IntroState::
pointerMove(int x, int y, int dx, int dy)
{
}


void NoDice::IntroState::
pointerClick(int x, int y, PointerAction action)
{
}


void NoDice::IntroState::
update(App& app)
{
}


void NoDice::IntroState::
draw(Video& video)
{
	glColor4fv(titleColour);
	m_menuFont.print(m_titlePos.x, m_titlePos.y, 1.0f, "No Dice!");

	for (int i = 0; i < menuCount; ++i)
	{
		if (i == m_selected)
			glColor4fv(selectedColour);
		else
			glColor4fv(unselectedColour);
		m_menuFont.print(entry[i].pos.x, entry[i].pos.y, 1.0f, entry[i].title);
	}
}


