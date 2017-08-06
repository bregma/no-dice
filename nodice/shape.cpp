/**
 * @file nodice/shape.cpp
 * @brief Implemntation of the nodice/shape module.
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
#include "nodice/shape.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include "nodice/d4.h"
#include "nodice/d6.h"
#include "nodice/d8.h"
#include "nodice/d12.h"
#include "nodice/d20.h"
#include <vector>


NoDice::Shape::
Shape(const std::string& name,
      const Colour&      default_colour)
: name_(name)
, default_colour_(default_colour)
{
}


NoDice::Shape::
~Shape()
{
}


const std::string& NoDice::Shape::
name() const
{
  return name_;
}


const NoDice::Colour& NoDice::Shape::
default_colour() const
{
  return default_colour_;
}


int NoDice::Shape::
score()
{
  return 0;
}


/**
 * Chooses a random die from a bag.
 */
NoDice::ShapePtr NoDice::
choose_a_shape()
{
  using ShapeBag = std::vector<ShapePtr>;

  static ShapeBag s_shapeBag {
      ShapePtr(new D4), ShapePtr(new D6), ShapePtr(new D8), ShapePtr(new D12), ShapePtr(new D20)
  };

  int r = (rand() >> 2) % s_shapeBag.size();
  return s_shapeBag.at(r);
}

/**
 * Generates the vertex buffer entries for a triangle.
 * @param[in]  vertex Array of vertexes 
 * @param[in]  index  Index of 3 vertexes in CCW order
 * @param[out] buffer Output buffer
 *
 * Calculates the normals at each vertex and copies the vertexes and normals
 * into the vertex buffer in an interleaved { V V V N N N } fashion.
 *
 * @warning The index must provide vertexes in the correct CCW winding order.
 * @warning The output @p buffer must be large enough to hold 18 floats.
 */
void NoDice::
triangle(const NoDice::vec3 vertex[], const int index[3], GLfloat*& buffer)
{
  vec3 v1 = (vertex[index[1]] - vertex[index[0]]);
  vec3 v2 = (vertex[index[2]] - vertex[index[0]]);
  vec3 normal = v1.cross(v2);

  buffer = std::copy(vertex[index[0]].begin(), vertex[index[0]].end(), buffer);
  buffer = std::copy(normal.begin(), normal.end(), buffer);
  buffer = std::copy(vertex[index[1]].begin(), vertex[index[1]].end(), buffer);
  buffer = std::copy(normal.begin(), normal.end(), buffer);
  buffer = std::copy(vertex[index[2]].begin(), vertex[index[2]].end(), buffer);
  buffer = std::copy(normal.begin(), normal.end(), buffer);
}

/**
 * Generates the vertex buffer entries for a pentagon.
 * @param[in]  vertex Array of vertexes 
 * @param[in]  index  Index of 5 vertexes in CCW order
 * @param[out] buffer Output buffer
 *
 * Calculates the normals at each vertex and copies the vertexes and normals
 * into the vertex buffer in an interleaved { V V V N N N } fashion.
 *
 * @warning The index must provide vertexes in the correct CCW winding order.
 * @warning The output @p buffer must be large enough to hold 30 floats.
 */
void NoDice::
pentagon(const NoDice::vec3 vertex[], const int index[5], GLfloat*& buffer)
{
  vec3 v1 {vertex[index[1]] - vertex[index[0]]};
  vec3 v2 {vertex[index[2]] - vertex[index[0]]};
  vec3 normal = v1.cross(v2);

  buffer = std::copy(vertex[index[0]].begin(), vertex[index[0]].end(), buffer);
  buffer = std::copy(normal.begin(), normal.end(), buffer);
  buffer = std::copy(vertex[index[1]].begin(), vertex[index[1]].end(), buffer);
  buffer = std::copy(normal.begin(), normal.end(), buffer);
  buffer = std::copy(vertex[index[2]].begin(), vertex[index[2]].end(), buffer);
  buffer = std::copy(normal.begin(), normal.end(), buffer);
  buffer = std::copy(vertex[index[0]].begin(), vertex[index[0]].end(), buffer);
  buffer = std::copy(normal.begin(), normal.end(), buffer);
  buffer = std::copy(vertex[index[2]].begin(), vertex[index[2]].end(), buffer);
  buffer = std::copy(normal.begin(), normal.end(), buffer);
  buffer = std::copy(vertex[index[3]].begin(), vertex[index[3]].end(), buffer);
  buffer = std::copy(normal.begin(), normal.end(), buffer);
  buffer = std::copy(vertex[index[0]].begin(), vertex[index[0]].end(), buffer);
  buffer = std::copy(normal.begin(), normal.end(), buffer);
  buffer = std::copy(vertex[index[3]].begin(), vertex[index[3]].end(), buffer);
  buffer = std::copy(normal.begin(), normal.end(), buffer);
  buffer = std::copy(vertex[index[4]].begin(), vertex[index[4]].end(), buffer);
  buffer = std::copy(normal.begin(), normal.end(), buffer);
}


