/**
 * @file nodice/object.cpp
 * @brief Implemntation of the no-dice object module.
 *
 * Copyright 2009, 2010 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
#include "nodice/object.h"

#include <cstdlib>
#include "nodice/video.h"


namespace
{
	static const int x_spin_speed = 1;
	static const int y_spin_speed = 6;
}


NoDice::Object::
Object(const ShapePtr shape)
: m_shape(shape)
, m_colour(m_shape->defaultColour())
, m_normalColour(m_colour)
, m_highlightColour(1.0f, 0.8f, 0.2f, 0.5f)
, m_xrot(std::rand() % 180), m_yrot((std::rand()>>2) % 90)
{
}


NoDice::Object::
~Object()
{
}


/**
 * The default type of an object is the name of it's shape.
 * Non-base objects can do something fancier.
 *
 * The name type allows two different objects to be compared for, say, the
 * purpose of matching.
 */
const std::string& NoDice::Object::
type() const
{
	return m_shape->name();
}


void NoDice::Object::
setHighlight(bool toggle)
{
	if (toggle)
		m_colour = m_highlightColour;
	else
		m_colour = m_normalColour;
}


void NoDice::Object::
update() 
{
	m_xrot = (m_xrot + x_spin_speed) % 360;
	m_yrot = (m_yrot + y_spin_speed) % 360;
}


void NoDice::Object::
draw() const
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glRotatef(float(m_xrot), 1.0f, 0.0f, 0.0f);
	glRotatef(float(m_yrot), 0.0f, 1.0f, 0.0f);

	glColor4fv(m_colour.rgba);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, m_colour.rgba);
	m_shape->draw();
	glPopMatrix();
}

