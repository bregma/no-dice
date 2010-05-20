/**
 * @file nodice/board.cpp
 * @brief Implemntation of the no-dice board module.
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
#include "nodice/board.h"

#include <iostream>
#include "nodice/config.h"
#include "nodice/object.h"

namespace
{
	static float swap_factor = 10.0f;
} // anonymous namespace


NoDice::Board::
Board(const NoDice::Config& config)
: m_config(config)
, m_state(state_idle)
{
	for (int y = 0; y < m_config.boardSize(); ++y)
	{
		for (int x = 0; x < m_config.boardSize(); ++x)
		{
			m_objects.push_back(ObjectPtr(new Object(NoDice::chooseAShape(),
																		 Vector3f(x * 2.0f, y * 2.0f, 0.0f))));
		}
	}
}


const NoDice::ObjectPtr&  NoDice::Board::
at(int x, int y) const
{
	return m_objects[x + y * m_config.boardSize()];
}


void NoDice::Board::
update()
{
	for (ObjectBag::iterator it = m_objects.begin();
			 it != m_objects.end();
			 ++it)
	{
		(*it)->update();
	}
	if (m_state == state_swapping)
	{
		m_swapStep += 0.5f;
		if (m_swapStep > swap_factor)
		{
			for (ObjectBag::iterator it = m_objects.begin();
			     it != m_objects.end();
			     ++it)
			{
				(*it)->setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
			}
			m_state = state_idle;
		}
	}
}


void NoDice::Board::
draw() const
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	for (int y = 0; y < m_config.boardSize(); ++y)
	{
		for (int x = 0; x < m_config.boardSize(); ++x)
		{
			at(x, y)->draw();
		}
	}
	glPopMatrix();
}


void NoDice::Board::
startSwap(int x1, int y1, int x2, int y2)
{
	ObjectPtr obj1 = at(x1, y1);
	ObjectPtr obj2 = at(x2, y2);
	Vector3f v1(float(x2-x1) / swap_factor, float(y2-y1) / swap_factor, 0.0f);
	Vector3f v2(float(x1-x2) / swap_factor, float(y1-y2) / swap_factor, 0.0f);
	obj1->setVelocity(v1);
	obj2->setVelocity(v2);
	m_swapStep = 0.0f;
	m_state = state_swapping;
}

bool NoDice::Board::
isSwapping() const
{
	return m_state == state_swapping;
}


/**
 * Looks for 3 (or more) matching objects in a row, horizontal or vertical.
 */
bool NoDice::Board::
findWins()
{
	int number_of_wins = 0;

	// check for 3-in-a-row
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 5; ++x)
		{
			const ObjectPtr& obj1 = at(x+0, y);
			const ObjectPtr& obj2 = at(x+1, y);
			const ObjectPtr& obj3 = at(x+2, y);
			if (obj1->type() == obj2->type() && obj2->type() == obj3->type())
			{
				obj1->startDisappearing();
				obj2->startDisappearing();
				obj3->startDisappearing();
				++number_of_wins;
			}
		}
	}

	// check for 3-in-a-column
	for (int x = 0; x < 8; ++x)
	{
		for (int y = 0; y < 5; ++y)
		{
			const ObjectPtr& obj1 = at(x, y+0);
			const ObjectPtr& obj2 = at(x, y+1);
			const ObjectPtr& obj3 = at(x, y+2);
			if (obj1->type() == obj2->type() && obj2->type() == obj3->type())
			{
				obj1->startDisappearing();
				obj2->startDisappearing();
				obj3->startDisappearing();
				++number_of_wins;
			}
		}
	}

	return number_of_wins > 0;
}
