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
	static const float swap_factor = 10.0f;
	static const float swap_step = 0.5f;
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
	if (findWins() > 0)
	{
		m_state = state_removing;
	}
}


NoDice::ObjectPtr&  NoDice::Board::
at(const NoDice::Vector2i& p)
{ return m_objects[p.x + p.y * m_config.boardSize()]; }


const NoDice::ObjectPtr&  NoDice::Board::
at(int x, int y) const
{ return m_objects[x + y * m_config.boardSize()]; }


void NoDice::Board::
update()
{
	for (ObjectBag::iterator it = m_objects.begin();
			 it != m_objects.end();
			 ++it)
	{
		(*it)->update();
	}

	switch (m_state)
	{
		case state_swapping:
		{
			m_swapStep += swap_step;
			if (m_swapStep > swap_factor)
			{
				at(m_swapObj[0]).swap(at(m_swapObj[1]));
				for (ObjectBag::iterator it = m_objects.begin();
						 it != m_objects.end();
						 ++it)
				{
					(*it)->setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
				}
				m_state = state_idle;
			}
			break;
		}

		case state_removing:
		{
			// Wait until all disappearing is finished.
			for (RemovalQueue::iterator it = m_removalQueue.begin();
					 it != m_removalQueue.end();
					 ++it)
			{
				if (!at(*it)->hasDisappeared())
					return;
			}

			m_state = state_falling;
			for (int x = 0; x < m_config.boardSize(); ++x)
			{
				int drop = 0;
				for (int y = 0; y < m_config.boardSize(); ++y)
				{
					if (at(x, y)->hasDisappeared())
					{
						++drop;
					}
					else if (drop > 0)
					{
						at(x, y)->startFalling(Vector3f(2.0f * x, 2.0f * (y - drop), 0.0f));
						m_fallingQueue.push_back(std::make_pair(Vector2i(x, y),
																										Vector2i(x, y - drop)));
					}
				}
				for (int y = 0; y < drop; ++y)
				{
					int new_y = m_config.boardSize() - y - 1;
					m_createQueue.push_back(Vector2i(x, new_y));
				}
			}
			m_removalQueue.clear();
			break;
		}

		case state_falling:
		{
			// Wait until all falling is finished.
			for (FallingQueue::iterator it = m_fallingQueue.begin();
					 it != m_fallingQueue.end();
					 ++it)
			{
				if (at((*it).first)->isFalling())
					return;
			}

			// Percolate removed jobbies out
			for (FallingQueue::iterator it = m_fallingQueue.begin();
					 it != m_fallingQueue.end();
					 ++it)
			{
				at((*it).first).swap(at((*it).second));
				ObjectPtr().swap(at((*it).first));
			}
			m_fallingQueue.clear();

			// Fill in the blanks.
			for (CreateQueue::iterator it = m_createQueue.begin();
					 it != m_createQueue.end();
					 ++it)
			{
				at(*it) = ObjectPtr(new Object(NoDice::chooseAShape(),
														Vector3f((*it).x * 2.0f, (*it).y * 2.0f, 0.0f)));
			}
			m_createQueue.clear();

			m_state = state_idle;
			break;
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
startSwap(NoDice::Vector2i pos1, NoDice::Vector2i pos2)
{
	m_swapObj[0] = pos1;
	m_swapObj[1] = pos2;
	ObjectPtr obj1 = at(pos1.x, pos1.y);
	ObjectPtr obj2 = at(pos2.x, pos2.y);
	obj1->setVelocity(Vector3f(float(pos2.x-pos1.x) / swap_factor,
	                           float(pos2.y-pos1.y) / swap_factor,
	                           0.0f));
	obj2->setVelocity(Vector3f(float(pos1.x-pos2.x) / swap_factor,
	                           float(pos1.y-pos2.y) / swap_factor,
	                           0.0f));
	m_swapStep = 0.0f;
	m_state = state_swapping;
}


void NoDice::Board::
unSwap()
{
	startSwap(m_swapObj[1], m_swapObj[0]);
}


bool NoDice::Board::
isSwapping() const
{
	return m_state == state_swapping;
}


bool NoDice::Board::
isReplacing() const
{
	return m_state == state_removing || m_state == state_falling;
}


/**
 * Looks for 3 (or more) matching objects in a row, horizontal or vertical.
 */
int NoDice::Board::
findWins()
{
	int number_of_wins = 0;

	// check for 3-in-a-row
	for (int y = 0; y < m_config.boardSize(); ++y)
	{
		for (int x = 0; x < m_config.boardSize()-2; ++x)
		{
			const Vector2i p1(x+0, y);
			const Vector2i p2(x+1, y);
			const Vector2i p3(x+2, y);
			const ObjectPtr& obj1 = at(p1);
			const ObjectPtr& obj2 = at(p2);
			const ObjectPtr& obj3 = at(p3);
			if (obj1->type() == obj2->type() && obj2->type() == obj3->type())
			{
				m_removalQueue.push_back(p1); obj1->startDisappearing();
				m_removalQueue.push_back(p2); obj2->startDisappearing();
				m_removalQueue.push_back(p3); obj3->startDisappearing();
				++number_of_wins;
			}
		}
	}

	// check for 3-in-a-column
	for (int x = 0; x < m_config.boardSize(); ++x)
	{
		for (int y = 0; y < m_config.boardSize()-2; ++y)
		{
			const Vector2i p1(x, y+0);
			const Vector2i p2(x, y+1);
			const Vector2i p3(x, y+2);
			const ObjectPtr& obj1 = at(p1);
			const ObjectPtr& obj2 = at(p2);
			const ObjectPtr& obj3 = at(p3);
			if (obj1->type() == obj2->type() && obj2->type() == obj3->type())
			{
				m_removalQueue.push_back(p1); obj1->startDisappearing();
				m_removalQueue.push_back(p2); obj2->startDisappearing();
				m_removalQueue.push_back(p3); obj3->startDisappearing();
				++number_of_wins;
			}
		}
	}

	if (number_of_wins > 0)
		m_state = state_removing;

	return number_of_wins;
}
