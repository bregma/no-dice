/**
 * @file nodice/playstate.h
 * @brief Public interface of the nodice/playstate module.
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
#ifndef NODICE_PLAYSTATE_H
#define NODICE_PLAYSTATE_H 1

#include "nodice/gamestate.h"

namespace NoDice
{

	class PlayState
	: public GameState
	{
	public:
		PlayState(Config& config);
		~PlayState();

		void update(App& app);
		void draw(Video& video);
	};

} // namespace noDice

#endif // NODICE_PLAYSTATE_H
