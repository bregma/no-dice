/**
 * @file nodice/object.cpp
 * @brief Implemntation of the no-dice object module.
 *
 * Copyright 2009, 2010, 2011 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
  static const float fade_rate = 20.0f;
  static const float move_rate = 10.0f;
}


NoDice::Object::
Object(const ShapePtr shape, const vec3& initialPosition)
: m_shape(shape)
, m_colour(m_shape->default_colour())
, m_normalColour(m_colour)
, m_highlightColour(1.0f, 0.8f, 0.2f, 0.5f)
, m_position(initialPosition)
, m_velocity(0.0f, 0.0f, 0.0f)
, m_isMoving(false)
, m_isDisappearing(false)
, m_fadeFactor(0.0f)
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
{ return m_shape->name(); }


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
  if (m_isDisappearing)
  {
    m_colour.a -= m_fadeFactor;
    if (this->hasDisappeared())
    {
      m_isDisappearing = false;
      m_fadeFactor = 0.0f;
    }
  }
  else
  {
    m_xrot = (m_xrot + x_spin_speed) % 360;
    m_yrot = (m_yrot + y_spin_speed) % 360;
  }
  if (m_isMoving)
  {
    if (!isFalling())
    {
      m_isMoving = false;
      m_velocity.set(0.0f, 0.0f, 0.0f);
    }
  }
  m_position += m_velocity;
}


int NoDice::Object::
score()
{
  return m_shape->score();
}


void NoDice::Object::
draw() const
{
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  glTranslatef(m_position.x, m_position.y, m_position.z);
  glRotatef(float(m_xrot), 1.0f, 0.0f, 0.0f);
  glRotatef(float(m_yrot), 0.0f, 1.0f, 0.0f);

  glColor4fv(m_colour.rgba);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, m_colour.rgba);
  m_shape->draw();
  glPopMatrix();
}


void NoDice::Object::
setVelocity(const vec3& velocity)
{ m_velocity = velocity; }
  

bool NoDice::Object::
hasDisappeared() const
{ return m_colour.a <= 0.0f; }


void NoDice::Object::
startDisappearing()
{
  m_isDisappearing = true;
  m_fadeFactor = m_colour.a / fade_rate;
}


bool NoDice::Object::
isFalling() const
{
  return m_position.distanceSquared(m_newPosition) > 0.01;
}


void NoDice::Object::
startFalling(const vec3& newPosition)
{
  m_isMoving = true;
  m_newPosition = newPosition;
  m_velocity = (m_newPosition - m_position) / move_rate;
}

