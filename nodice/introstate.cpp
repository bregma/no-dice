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
#include "nodice/fontcache.h"
#include "nodice/video.h"

namespace NoDice
{

IntroState::
IntroState(const Video& video)
: m_isActive(true)
, m_menuFont(FontCache::get("FreeSans", video.screenHeight() / 10))
, m_titlePos(0.25 * video.screenWidth(), 0.75 * video.screenHeight())
, m_optionsPos(0.25 * video.screenWidth(),
							 m_titlePos.y - 2.0f * m_menuFont.height())
, m_playPos(0.25 * video.screenWidth(), 
							 m_optionsPos.y - 1.5f * m_menuFont.height())
, m_quitPos(0.25 * video.screenWidth(), 
							 m_playPos.y - 1.5f * m_menuFont.height())
{
}

IntroState::
~IntroState()
{
}

void IntroState::
pause()
{
	m_isActive = false;
}


void IntroState::
resume()
{
	m_isActive = true;
}


void IntroState::
pointerMove(int x, int y, int dx, int dy)
{
}

void IntroState::
pointerClick(int x, int y, PointerAction action)
{
}

void IntroState::
update()
{
}

void IntroState::
draw(Video& video)
{
	static float titleColour[] = { 1.00f, 0.10f, 0.10f, 1.00f };
	static float selectedColour[] = { 0.40f, 0.40f, 1.00f, 0.80f };
	static float unselectedColour[] = { 0.20f, 0.20f, 0.80f, 0.80f };

	glColor4fv(titleColour);
	m_menuFont.print(m_titlePos.x, m_titlePos.y, 1.0f, "No Dice!");

	glColor4fv(unselectedColour);
	m_menuFont.print(m_optionsPos.x, m_optionsPos.y, 1.0f, "options");
	glColor4fv(selectedColour);
	m_menuFont.print(m_playPos.x, m_playPos.y, 1.0f, "play");
	glColor4fv(unselectedColour);
	m_menuFont.print(m_quitPos.x, m_quitPos.y, 1.0f, "quit");
}


} // namespace NoDice

