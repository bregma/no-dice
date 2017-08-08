/**
 * @file nodice/shaderprogram.cpp
 * @brief Private implemntation of the nodice/shaderprogram module.
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
#include "nodice/shaderprogram.h"

#include <iostream>
#include <map>
#include "nodice/opengl.h"
#include "nodice/shader.h"
#include <stdexcept>
#include <string>
#include <vector>


/**
 * Internal implemntation of the SHaderProgram.
 */
struct NoDice::ShaderProgram::Impl
{
  Impl()
  : program_(glCreateProgram())
  , is_linked_(false)
  { }

  ~Impl()
  {
    glDeleteProgram(program_);
  }

  using AttributeIndexMap = std::map<std::string, GLint>;
  using UniformIndexMap   = std::map<std::string, GLint>;

  GLuint            program_;
  bool              is_linked_;
  AttributeIndexMap attribute_index_map_;
  UniformIndexMap   uniform_index_map_;
};


/**
 * Constructs a shader program object.
 */
NoDice::ShaderProgram::
ShaderProgram()
: impl_(new Impl())
{
}


/**
 * Destroys the shader object.
 */
NoDice::ShaderProgram::
~ShaderProgram()
{
}


/**
 * Attaches a shader to the program.
 */
void NoDice::ShaderProgram::
attach(NoDice::Shader const& shader)
{
  glAttachShader(impl_->program_, shader.id());
  impl_->is_linked_ = false;
}


/**
 * Links the program.
 */
void NoDice::ShaderProgram::
link()
{
  glLinkProgram(impl_->program_);

  GLint status;
  glGetProgramiv(impl_->program_, GL_LINK_STATUS, &status);
  if (status == GL_FALSE)
  {
    GLint log_size;
    glGetProgramiv(impl_->program_, GL_INFO_LOG_LENGTH, &log_size);
    std::vector<char> buf(log_size);
    glGetProgramInfoLog(impl_->program_, log_size, NULL, &buf[0]);
    std::string message(buf.begin(), buf.end());
    throw std::runtime_error(message);
  }

  GLint uniform_count = 0;
  glGetProgramiv(impl_->program_, GL_ACTIVE_UNIFORMS, &uniform_count);
  if (uniform_count > 0)
  {
    std::cerr << "==smw> " << __PRETTY_FUNCTION__
              << " uniform_count = " << uniform_count << "\n";
    GLint max_uniform_name_len = 0;
    glGetProgramiv(impl_->program_,
                   GL_ACTIVE_UNIFORM_MAX_LENGTH,
                   &max_uniform_name_len);
    std::cerr << "==smw> " << __PRETTY_FUNCTION__
              << " max_uniform_name_len = " << max_uniform_name_len << "\n";
    std::vector<char> uniform_name(max_uniform_name_len);
    for (GLint i = 0; i < uniform_count; ++i)
    {
      GLsizei name_length;
      GLint   dimension;
      GLenum  type;
      glGetActiveUniform(impl_->program_,
                         i,
                         max_uniform_name_len,
                         &name_length,
                         &dimension,
                         &type,
                         &uniform_name[0]);
      std::string name(&uniform_name[0], name_length);
      GLint loc = glGetUniformLocation(impl_->program_, name.c_str());
      impl_->uniform_index_map_.insert(std::make_pair(name, loc));
      std::cerr << "==smw> " << __PRETTY_FUNCTION__ << " uniform: "
                << name
                << " type=" << type
                << " dim=" << dimension
                << " loc=" << loc
                << "\n";
    }
  }

  GLint attribute_count = 0;
  glGetProgramiv(impl_->program_, GL_ACTIVE_ATTRIBUTES, &attribute_count);
  if (attribute_count > 0)
  {
    std::cerr << "==smw> " << __PRETTY_FUNCTION__
              << " attribute_count = " << attribute_count << "\n";
    GLint max_attribute_name_length = 0;
    glGetProgramiv(impl_->program_,
                   GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
                   &max_attribute_name_length);
    std::cerr << "==smw> " << __PRETTY_FUNCTION__
              << " max_attribute_name_length = " << max_attribute_name_length << "\n";
    std::vector<char> attribute_name(max_attribute_name_length);
    for (int i = 0; i < attribute_count; ++i)
    {
      GLsizei name_length;
      GLint   dimension;
      GLenum  type;
      glGetActiveAttrib(impl_->program_,
                         i,
                         max_attribute_name_length,
                         &name_length,
                         &dimension,
                         &type,
                         &attribute_name[0]);
      std::string name(&attribute_name[0], name_length);
      GLint loc = glGetAttribLocation(impl_->program_, name.c_str());
      impl_->attribute_index_map_.insert(std::make_pair(name, loc));
      std::cerr << "==smw> " << __PRETTY_FUNCTION__
                << " attribute: "
                << name
                << " type=" << type
                << " dim=" << dimension
                << " loc=" << loc
                << "\n";
    }
  }
  impl_->is_linked_ = true;
}


/**
 * Makes the program the currect shader program in use.
 */
void NoDice::ShaderProgram::
activate()
{
  if (!impl_->is_linked_)
  {
    link();
  }
  glUseProgram(impl_->program_);
}


void NoDice::ShaderProgram::
deactivate()
{
  glUseProgram(0);
}


void NoDice::ShaderProgram::
set_uniform(const std::string& name, float value)
{
  auto it = impl_->uniform_index_map_.find(name);
  if (it == impl_->uniform_index_map_.end())
  {
    std::cerr << "WARNING: uniform \"" << name << "\" not found.\n";
    return;
  }
  glUniform1f(it->second, value);
}


void NoDice::ShaderProgram::
set_uniform(const std::string& name, float v1, float v2, float v3)
{
  auto it = impl_->uniform_index_map_.find(name);
  if (it == impl_->uniform_index_map_.end())
  {
    std::cerr << "WARNING: uniform \"" << name << "\" not found.\n";
    return;
  }
  glUniform3f(it->second, v1, v2, v3);
}


void NoDice::ShaderProgram::
set_uniform(const std::string& name, const NoDice::mat4& mat)
{
  auto it = impl_->uniform_index_map_.find(name);
  if (it == impl_->uniform_index_map_.end())
  {
    std::cerr << "WARNING: uniform \"" << name << "\" not found.\n";
    return;
  }
  glUniformMatrix4fv(it->second, 1, GL_FALSE, mat.array);
}


void NoDice::ShaderProgram::
set_attribute(const std::string& name, int size, int stride, void const* ptr)
{
  auto it = impl_->attribute_index_map_.find(name);
  if (it == impl_->attribute_index_map_.end())
  {
    std::cerr << "WARNING: attribute \"" << name << "\" not found.\n";
    return;
  }
  glEnableVertexAttribArray(it->second);
  glVertexAttribPointer(it->second, size, GL_FLOAT, GL_FALSE, stride, ptr);
}

