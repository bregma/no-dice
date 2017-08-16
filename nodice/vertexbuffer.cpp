/**
 * @file vertexbuffer.cpp
 * @brief Implemntation of the VertexBuffer module.
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
#include "nodice/vertexbuffer.h"

NoDice::VertexBuffer::
VertexBuffer(GLenum target, GLsizeiptr size, GLvoid const* data, GLenum usage)
: target_(target)
{
  glGenBuffers(1, &vbo_);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glGenBuffers()");
  bind();
  glBufferData(target_, size, data, usage);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBufferData()");

}


NoDice::VertexBuffer::
~VertexBuffer()
{
  unbind();
  glDeleteBuffers(1, &vbo_);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glDeleteBuffers()");
}


void NoDice::VertexBuffer::
bind() const
{
  glBindBuffer(target_, vbo_);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindBuffer()");
}


void NoDice::VertexBuffer::
copy_in(GLintptr offset, GLsizeiptr size, GLvoid const* data)
{
  glBufferSubData(target_, offset, size, data);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBufferSubData()");
}


void NoDice::VertexBuffer::
unbind() const
{
  glBindBuffer(target_, 0);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindBuffer()");
}
