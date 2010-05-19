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
#include "nodice/object.h"
#include "nodice/video.h"


NoDice::Board::
Board(const NoDice::Config& config)
{
	for (int i = 0; i < 64; ++i)
	{
		m_objects.push_back(ObjectPtr(new Object(NoDice::chooseAShape())));
	}
}


const NoDice::ObjectPtr&  NoDice::Board::
at(int x, int y) const
{
	return m_objects[8*x + y];
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
	findWins();
}


void NoDice::Board::
draw() const
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	for (int y = 0; y < 8; ++y)
	{
		glPushMatrix();
		for (int x = 0; x < 8; ++x)
		{
			at(x, y)->draw();
			glTranslatef(2.0f, 0.0f, 0.0f);
		}
		glPopMatrix();
		glTranslatef(0.0f, 2.25f, 0.0f);
	}
	glPopMatrix();
}


bool NoDice::Board::
findWins()
{
	bool win_found = false;

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
				win_found = true;
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
				win_found = true;
			}
		}
	}

	return win_found;
}
