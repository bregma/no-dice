/**
 * @file openglmesh.cpp
 * @brief Implemntation of the No-Dice OpenGL Mesh module.
 */
/*
 * Copyright 2017 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
#include "nodice/openglmesh.h"


NoDice::OpenGLMesh::
OpenGLMesh()
: vao{}
, vbo{ 0, 0 }
{
  glGenVertexArrays(1, &this->vao);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glGenVertexArrays()");
}


NoDice::OpenGLMesh::
~OpenGLMesh()
{
  glBindVertexArray(this->vao);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindVertexArray()");

  for (auto b: this->vbo)
  {
    if (b)
    {
      glDeleteBuffers(1, &b);
      check_gl_error(std::string{__PRETTY_FUNCTION__} + " glDeleteBuffers()");
    }
  }

  glBindVertexArray(0);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindVertexArray()");
  glDeleteVertexArrays(1, &this->vao);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glDeleteVertexArrays()");
}


void NoDice::OpenGLMesh::
store_vertex_data(float const* data)
{
  glBindVertexArray(this->vao);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindVertexArray()");

  glGenBuffers(1, &this->vbo[0]);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glGenBuffers()");
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindBuffer()");
  glBufferData(GL_ARRAY_BUFFER,
               this->vertex_count_ * this->vertex_stride_bytes_,
               static_cast<const void*>(data),
               GL_STATIC_DRAW);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBufferData()");

  glBindVertexArray(0);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindVertexArray()");
}


void NoDice::OpenGLMesh::
reset_vertex_data()
{
  glBindVertexArray(this->vao);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindVertexArray()");

  if (this->vbo[0])
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindBuffer()");
    this->vbo[0] = 0;
  }

  glBindVertexArray(0);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindVertexArray()");
}


void NoDice::OpenGLMesh::
store_index_data(std::uint16_t const* data)
{
  glBindVertexArray(this->vao);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindVertexArray()");

  glGenBuffers(1, &this->vbo[0]);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glGenBuffers()");
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbo[1]);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindBuffer()");
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               this->index_count_ * sizeof(std::uint16_t),
               static_cast<const void*>(data),
               GL_STATIC_DRAW);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBufferData()");

  glBindVertexArray(0);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindVertexArray()");
}


void NoDice::OpenGLMesh::
reset_index_data()
{
  glBindVertexArray(this->vao);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindVertexArray()");

  if (this->vbo[1])
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindBuffer()");
    this->vbo[1] = 0;
  }

  glBindVertexArray(0);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindVertexArray()");
}


void NoDice::OpenGLMesh::
draw_indexed()
{
  glBindVertexArray(this->vao);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindVertexArray()");

  glDrawElements(GL_TRIANGLES, this->index_count_, GL_UNSIGNED_SHORT, NULL);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glDrawElements()");

  glBindVertexArray(0);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindVertexArray()");
}


void NoDice::OpenGLMesh::
draw_direct()
{
  glBindVertexArray(this->vao);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindVertexArray()");

  glDrawArrays(GL_TRIANGLES, 0, this->vertex_count_);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glDrawArrays()");

  glBindVertexArray(0);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindVertexArray()");
}

