/**
 * @file nodice/object.h
 * @brief Public interface of the nodice/object module.
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
#ifndef NODICE_OBJECT_H
#define NODICE_OBJECT_H 1

namespace NoDice
{
  class Shape;

  /**
   * An instance of a drawable object.
   *
   * A drawable object instance has a shape, a transform (modelview matrix),
   * a colour, and um, I dunno, other state.
   */
  class Object
  {
  public:
    Object(const Shape* shape);
    ~Object();

  private:
    Object(const Object&);
    Object& operator=(const Object&);

  private:
    const Shape* m_shape;
  };
} // namespace NoDice

#endif // NODICE_OBJECT_H