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
  this->activate();

  for (auto b: this->vbo)
  {
    if (b)
    {
      glDeleteBuffers(1, &b);
      check_gl_error(std::string{__PRETTY_FUNCTION__} + " glDeleteBuffers()");
    }
  }

  this->deactivate();
  glDeleteVertexArrays(1, &this->vao);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glDeleteVertexArrays()");
}


void NoDice::OpenGLMesh::
store_vertex_data(float const* data)
{
  this->activate();

  glGenBuffers(1, &this->vbo[0]);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glGenBuffers()");
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindBuffer()");
  glBufferData(GL_ARRAY_BUFFER,
               this->vertex_count_ * this->vertex_stride_bytes_,
               static_cast<const void*>(data),
               GL_STATIC_DRAW);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBufferData()");

  this->deactivate();
}


void NoDice::OpenGLMesh::
reset_vertex_data()
{
  this->activate();

  if (this->vbo[0])
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindBuffer()");
    this->vbo[0] = 0;
  }

  this->deactivate();
}


void NoDice::OpenGLMesh::
store_index_data(std::uint16_t const* data)
{
  this->activate();

  glGenBuffers(1, &this->vbo[1]);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glGenBuffers()");
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbo[1]);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindBuffer()");
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               this->index_count_ * sizeof(std::uint16_t),
               static_cast<const void*>(data),
               GL_STATIC_DRAW);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBufferData()");

  this->deactivate();
}


void NoDice::OpenGLMesh::
reset_index_data()
{
  this->activate();

  if (this->vbo[1])
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindBuffer()");
    this->vbo[1] = 0;
  }

  this->deactivate();
}


void NoDice::OpenGLMesh::
activate()
{
  glBindVertexArray(this->vao);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindVertexArray()");
}


bool NoDice::OpenGLMesh::
is_active_p() const
{
  GLuint active_vao;
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, reinterpret_cast<GLint*>(&active_vao));
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glGetIntegerv()");
  return active_vao == this->vao;
}


void NoDice::OpenGLMesh::
deactivate()
{
  glBindVertexArray(0);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindVertexArray()");
}


void NoDice::OpenGLMesh::
draw_indexed()
{
  this->activate();

  glDrawElements(GL_TRIANGLES, this->index_count_, GL_UNSIGNED_SHORT, NULL);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glDrawElements()");

  glBindVertexArray(0);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindVertexArray()");

  this->deactivate();
}


void NoDice::OpenGLMesh::
draw_direct()
{
  this->activate();

  glDrawArrays(GL_TRIANGLES, 0, this->vertex_count_);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glDrawArrays()");

  glBindVertexArray(0);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindVertexArray()");

  this->deactivate();
}

