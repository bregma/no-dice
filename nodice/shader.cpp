/**
 * @file nodice/shader.cpp
 * @brief Private implemntation of the nodice/shader module.
 *
 * Copyright 2013 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
#include "nodice/shader.h"


static std::string const
find_shader_source(std::string const& source_file_name)
{
  return source_file_name;
}


static std::string const
load_shader(std::string const& source_file_name)
{
  find_shader_source(source_file_name);
  return "int main() { }";
}


NoDice::Shader::
Shader(GLenum shader_type, std::string const& source_file_name)
: shader_(glCreateShader(shader_type))
{
  std::string const source = load_shader(source_file_name);
  char const* ptrs[1] = { source.c_str() };
  glShaderSource(shader_, 1, ptrs, nullptr);
  glCompileShader(shader_);
}


NoDice::Shader::
~Shader()
{
  glDeleteShader(shader_);
}
