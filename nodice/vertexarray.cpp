/**
 * @file vertexarray.cpp
 * @brief Implemntation of the VertexArray module.
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
#include "nodice/vertexarray.h"

#include <iostream>


NoDice::VertexArray::
VertexArray()
{
  glGenVertexArrays(1, &vao_);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glGenVertexArrays()");
  bind();
}


NoDice::VertexArray::
~VertexArray()
{
  unbind();
  glDeleteVertexArrays(1, &vao_);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glDeleteVertexArrays()");
}


void NoDice::VertexArray::
bind() const
{
  glBindVertexArray(vao_);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindVertexArray()");
}


void NoDice::VertexArray::
unbind() const
{
  glBindVertexArray(0);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindVertexArray()");
}

