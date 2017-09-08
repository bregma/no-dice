/**
 * @file nodice/d4.cpp
 * @brief Implemntation of the no-dice d4 module.
 */
/*
 * Copyright 2010, 2011, 2017 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
#include "nodice/d4.h"

#include <cmath>
#include <cstdlib>
#include "nodice/maths.h"


namespace
{
  const GLfloat half = 0.5773502692; // for tetrahedron in unit sphere

  const int triangles_per_face = 1;
  const int row_width          = NoDice::coords_per_vertex + NoDice::coords_per_normal;

  const NoDice::vec3 vertex[] =
  {
    { half,  half,  half },
    {-half,  half, -half },
    {-half, -half,  half },
    { half, -half, -half }
  };

  // List of vertexes making up a face.
  using FaceVertexIndex = int[3];

  // Table of vertexes making up the faces of the tetrahedron.
  const FaceVertexIndex indexes[] =
  {
    { 1, 2, 0 },
    { 2, 3, 0 },
    { 3, 2, 1 },
    { 0, 3, 1 }
  };

  const int num_faces    = sizeof(indexes) / sizeof(FaceVertexIndex);
  const int vertex_count = num_faces * triangles_per_face * NoDice::vertexes_per_triangle;
} // anonymous namespace

NoDice::D4::
D4()
: Shape("d4", NoDice::Colour(1.0f, 1.0f, 0.0f, 0.60f))
, vbo_(GL_ARRAY_BUFFER, row_width * vertex_count * sizeof(GLfloat), nullptr, GL_STATIC_DRAW)
{
  // A temporary buffer with which the VBO will be initialized
  GLfloat shape[row_width * vertex_count];
  GLfloat* p = shape;
  for (int i = 0; i < num_faces; ++i)
  {
    triangle(vertex, indexes[i], p);
  }

  VertexBufferBinding vbo_binding(vbo_);
  vbo_.copy_in(0, sizeof(shape), shape);

  // Remember the size of the VBO.
  vertex_count_ = vertex_count;
} 


NoDice::D4::
~D4()
{ } 


int NoDice::D4::
score()
{
  return std::rand() % 4 + 1;
}


void NoDice::D4::
draw() const
{
  static const int stride = row_width * sizeof(GLfloat);
  static const GLfloat* shape_verteces = 0;
  static const GLfloat* shape_normals = shape_verteces + coords_per_vertex;

  glEnable(GL_NORMALIZE);
  glDisable(GL_CULL_FACE); // for transparency -- move to object
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  VertexBufferBinding vbo_binding(vbo_);
  glNormalPointer(GL_FLOAT, stride, shape_normals);
  glVertexPointer(coords_per_vertex, GL_FLOAT, stride, shape_verteces);
  glDrawArrays(GL_TRIANGLES, 0, vertex_count_);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisable(GL_NORMALIZE);
}

