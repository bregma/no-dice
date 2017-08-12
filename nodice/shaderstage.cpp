/**
 * @file nodice/shaderstage.cpp
 * @brief Private implemntation of the shader stage module.
 */
/*
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
#include "nodice/shaderstage.h"

#include <fstream>
#include <iterator>
#include "nodice/config.h"
#include <stdexcept>
#include <sys/stat.h>
#include <unistd.h>


/**
 * Finds the named shader source file on the configured asset path.
 * @param[in] config           The current configuration.
 * @param[in] source_file_name The name of the shader source file.
 */
static std::string
find_shader_source(NoDice::Config const& config,
                   std::string const&    source_file_name)
{
  for (auto const& s: config.asset_search_path())
  {
    std::string file_name = s + "/" + source_file_name;
    struct stat fs;
    int sstat = stat(file_name.c_str(), &fs);
    if (0 == sstat)
    {
      return file_name;
    }
  }
  throw std::runtime_error("can not find shader '" + source_file_name + "'");
}


/**
 * Loads the named shader source file into a string buffer.
 *
 * @param[in] config           The current configuration.
 * @param[in] source_file_name The name of the shader source file.
 */
static std::string const
load_shader(NoDice::Config const& config,
            std::string const&    source_file_name)
{
  std::ifstream in(find_shader_source(config, source_file_name));
  return std::string(std::istreambuf_iterator<char>(in), 
                     std::istreambuf_iterator<char>());
}


/**
 * Constructs a shader object from a named GLSL source file.
 * @param[in] config           The current configuration.
 * @param[in] shader_type      The type of the shader to compile:  should be one
 *                             of @c GL_VERTEX_SHADER or @c GL_FRAGMENT_SHADER.
 * @param[in] source_file_name The name of the shader source file.
 */
NoDice::ShaderStage::
ShaderStage(NoDice::Config const& config,
       GLenum                shader_type,
       std::string const&    source_file_name)
: shader_(glCreateShader(shader_type))
{
  std::string const source = load_shader(config, source_file_name);
  char const* ptrs[1] = { source.c_str() };
  glShaderSource(shader_, 1, ptrs, nullptr);
  glCompileShader(shader_);

  GLint status;
  glGetShaderiv(shader_, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE)
  {
    GLint log_size;
    glGetShaderiv(shader_, GL_INFO_LOG_LENGTH, &log_size);
    std::vector<char> buf(log_size);
    glGetShaderInfoLog(shader_, log_size, NULL, &buf[0]);
    std::string message(buf.begin(), buf.end());
    throw std::runtime_error(message);
  }
}


/**
 * Destroys the shader object.
 */
NoDice::ShaderStage::
~ShaderStage()
{
  glDeleteShader(shader_);
}


/**
 * Gets the internal identifier for the shader.
 */
GLint NoDice::ShaderStage::
id() const
{
  return shader_;
}

