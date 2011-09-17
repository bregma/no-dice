/**
 * @file nodice/gamestate.cpp
 * @brief Implemntation of the NoDice gamestate module.
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
#include "gamestate.h"

#include <iostream>


NoDice::GameState::
GameState(Config& config)
: m_config(config)
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
key(SDL_keysym keysym NODICE_UNUSED)
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

