/**
 * @file nodice/shape.h
 * @brief Public interface of the nodice/shape module.
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
#ifndef NODICE_SHAPE_H
#define NODICE_SHAPE_H 1

#include "nodice/colour.h"
#include <string>
#include <tr1/memory>
#include "nodice/maths.h"
#include "nodice/video.h"


namespace NoDice
{
  /**
   * Base class for all drawable shapes.
   */
  class Shape
  {
  public:
    /** Constructs a shape base object. */
    Shape(const std::string& name,
    			const Colour&      defaultColour);

    /** Destroys a shape. */
    virtual ~Shape() = 0;

    /** Gets the name of the shape. */
    const std::string& name() const;

    /** Gets the default colour for the shape. */
    const Colour& defaultColour() const;

    /** Renders the shape. */
    virtual void draw() const = 0;

  private:
    std::string  m_name;
		Colour       m_defaultColour;
  };

  /** Points to a shape. */
  typedef std::tr1::shared_ptr<Shape> ShapePtr;

  /** Randomly chooses a shape from its bag. */
  ShapePtr chooseAShape();

  /** Generates a triangle. */
  void triangle(const Vector3f vertexes[],
	              const int indexes[3],
	              GLfloat*& buffer);

  /** Generates a pentagon. */
  void pentagon(const Vector3f vertexes[],
                const int indexes[5],
                GLfloat*& buffer);
} // namespace NoDice

#endif // NODICE_SHAPE_H
