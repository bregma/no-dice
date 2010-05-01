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
IntroState()
: m_isActive(true)
{
}

IntroState::
~IntroState()
{
}

void IntroState::
pointerMove(int x, int y, int dx, int dy)
{
	std::cerr << "==smw> " << __PRETTY_FUNCTION__ << "\n";
}

void IntroState::
pointerClick(int x, int y, PointerAction action)
{
	std::cerr << "==smw> " << __PRETTY_FUNCTION__ << "\n";
	m_isActive = (action != pointerUp);
}

void IntroState::
update()
{
}

void IntroState::
draw(Video& video)
{
	Font& font = FontCache::get("FreeSans", 18);
	float tx = 0.25 * video.screenWidth();
	float ty = video.screenHeight() - (0.25 * video.screenHeight());
	glColor4f(0.25f, 0.5f, 1.0f, 0.5f);
	if (m_isActive)
		font.print(tx, ty, 2.0f, "No Dice!");
	else
		font.print(tx, ty, 2.0f, "No Dice (inactive)!");
}


} // namespace NoDice

