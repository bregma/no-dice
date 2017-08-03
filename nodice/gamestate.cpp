/**
 * @file nodice/gamestate.cpp
 * @brief Implemntation of the NoDice gamestate module.
 *
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
#include "nodice_config.h"
#include "nodice/gamestate.h"

#include <iostream>


NoDice::GameState::
GameState(NoDice::App* app)
: app_(app)
{
}


NoDice::GameState::
~GameState()
{
}


void NoDice::GameState::
pause()
{
	std::cerr << __PRETTY_FUNCTION__ << "\n";
}


void NoDice::GameState::
resume()
{
	std::cerr << __PRETTY_FUNCTION__ << "\n";
}


void NoDice::GameState::
key(SDL_Keysym keysym NODICE_UNUSED)
{
	std::cerr << __PRETTY_FUNCTION__ << "\n";
}


void NoDice::GameState::
pointerMove(int x NODICE_UNUSED,
            int y NODICE_UNUSED,
            int dx NODICE_UNUSED,
            int dy NODICE_UNUSED)
{
	std::cerr << __PRETTY_FUNCTION__ << "\n";
}


void NoDice::GameState::
pointerClick(int x NODICE_UNUSED,
             int y NODICE_UNUSED,
             PointerAction action NODICE_UNUSED)
{
	std::cerr << __PRETTY_FUNCTION__ << "\n";
}

