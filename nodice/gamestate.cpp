/**
 * @file gamestate.cpp
 * @brief Implemntation of the gamestate module.
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
#include "gamestate.h"

#include <iostream>


namespace NoDice
{

GameState::
~GameState()
{
}


void GameState::
pause()
{
	std::cerr << __PRETTY_FUNCTION__ << "\n";
}


void GameState::
resume()
{
	std::cerr << __PRETTY_FUNCTION__ << "\n";
}


void GameState::
pointerMove(int x, int y, int dx, int dy)
{
	std::cerr << __PRETTY_FUNCTION__ << "\n";
}

void GameState::
pointerClick(int x, int y, PointerAction action)
{
	std::cerr << __PRETTY_FUNCTION__ << "\n";
}

} // namespace NoDice
