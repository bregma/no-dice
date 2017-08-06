/**
 * @file nodice/shape.h
 * @brief Public interface of the nodice/shape module.
 */
/*
 * Copyright 2009, 2010, 2011, 2017 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
 *
 * This file is part of no-dice.
 *
 * No-dice is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * No-dice is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with no-dice.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef NODICE_SHAPE_H
#define NODICE_SHAPE_H 1

#include "nodice/colour.h"
#include <string>
#include <memory>
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
    Shape(std::string const& name,
          Colour const&      default_colour);

    /** Destroys a shape. */
    virtual ~Shape() = 0;

    /** Gets the name of the shape. */
    std::string const&
    name() const;

    /** Gets the default colour for the shape. */
    Colour const&
    default_colour() const;

    /** Gives the base score for the shape. */
    virtual int
    score();

    /** Renders the shape. */
    virtual void
    draw() const = 0;

  private:
    std::string  name_;
    Colour       default_colour_;
  };


  /** Points to a shape. */
  using ShapePtr = std::shared_ptr<Shape>;


  /** Randomly chooses a shape from its bag. */
  ShapePtr
  choose_a_shape();

  /** Generates a triangle. */
  void
  triangle(vec3 const vertexes[],
           int const  indexes[3],
           GLfloat*&  buffer);

  /** Generates a pentagon. */
  void
  pentagon(vec3 const vertexes[],
           int const  indexes[5],
           GLfloat*&  buffer);
} // namespace NoDice

#endif // NODICE_SHAPE_H
