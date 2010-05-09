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

#include "nodice/video.h"


NoDice::Object::
Object(const ShapePtr shape)
: m_shape(shape)
, m_colour(m_shape->defaultColour())
{
}


NoDice::Object::
~Object()
{
}


void NoDice::Object::
draw() const
{
	glColor4fv(m_colour.rgba);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, m_colour.rgba);
	m_shape->draw();
}

