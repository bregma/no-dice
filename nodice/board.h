/**
 * @file nodice/board.h
 * @brief Public interface of the nodice/board module.
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
#ifndef NODICE_BOARD_H
#define NODICE_BOARD_H 1

#include "nodice/config.h"
#include "nodice/maths.h"
#include "nodice/object.h"
#include <vector>


namespace NoDice
{
	class Config;

	/**
	 * The playing surface.
	 */
	class Board
	{
	public:
		Board(const Config& config);

		ObjectPtr& at(int x, int y);
		const ObjectPtr& at(int x, int y) const;

		void update();
		void draw() const;

		void startSwap(Vector2i objPos1, Vector2i objPos2);
		void unSwap();
		bool isSwapping() const;

		bool findWins();

	private:
		typedef std::vector<ObjectPtr> ObjectBag;
		enum State
		{
			state_idle,
			state_swapping,
			state_removing,
			state_falling
		};

		Config    m_config;
		ObjectBag m_objects;
		State     m_state;
		float     m_swapStep;
		Vector2i  m_swapObj[2];
	};
} // namespace NoDice

#endif // NODICE_BOARD_H
