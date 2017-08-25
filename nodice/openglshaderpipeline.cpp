/**
 * @file openglshaderpipeline.cpp
 * @brief Implemntation of the No-Dice OpenGL shader pipeline module.
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
#include "nodice/openglshaderpipeline.h"

#include "nodice/shaderstage.h"
#include <stdexcept>
#include <vector>


NoDice::OpenGLShaderPipeline::
OpenGLShaderPipeline(NoDice::ShaderPipeline::Id id)
: ShaderPipeline(id)
, program(glCreateProgram())
{
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glCreateProgram()");
}


NoDice::OpenGLShaderPipeline::
~OpenGLShaderPipeline()
{ }


void NoDice::OpenGLShaderPipeline::
attach_stage_p(NoDice::ShaderStage const& shader_stage)
{
  glAttachShader(this->program, shader_stage.id());
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glAttachShader()");
}


void NoDice::OpenGLShaderPipeline::
link_p()
{
  glLinkProgram(this->program);

  GLint status;
  glGetProgramiv(this->program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE)
  {
    GLint log_size;
    glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &log_size);
    std::vector<char> buf(log_size);
    glGetProgramInfoLog(this->program, log_size, NULL, &buf[0]);
    std::string message(buf.begin(), buf.end());
    throw std::runtime_error(message);
  }

  GLint uniform_count = 0;
  glGetProgramiv(this->program, GL_ACTIVE_UNIFORMS, &uniform_count);
  if (uniform_count > 0)
  {
    std::cerr << "==smw> " << __PRETTY_FUNCTION__
              << " uniform_count = " << uniform_count << "\n";
    GLint max_uniform_name_len = 0;
    glGetProgramiv(this->program,
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
      glGetActiveUniform(this->program,
                         i,
                         max_uniform_name_len,
                         &name_length,
                         &dimension,
                         &type,
                         &uniform_name[0]);
      std::string name(&uniform_name[0], name_length);
      GLint loc = glGetUniformLocation(this->program, name.c_str());
      this->uniform_index_map.insert(std::make_pair(name, loc));
      std::cerr << "==smw> " << __PRETTY_FUNCTION__ << " uniform: "
                << name
                << " type=" << type
                << " dim=" << dimension
                << " loc=" << loc
                << "\n";
    }
  }

  GLint attribute_count = 0;
  glGetProgramiv(this->program, GL_ACTIVE_ATTRIBUTES, &attribute_count);
  if (attribute_count > 0)
  {
    std::cerr << "==smw> " << __PRETTY_FUNCTION__
              << " attribute_count = " << attribute_count << "\n";
    GLint max_attribute_name_length = 0;
    glGetProgramiv(this->program,
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
      glGetActiveAttrib(this->program,
                         i,
                         max_attribute_name_length,
                         &name_length,
                         &dimension,
                         &type,
                         &attribute_name[0]);
      std::string name(&attribute_name[0], name_length);
      GLint loc = glGetAttribLocation(this->program, name.c_str());
      this->attribute_index_map.insert(std::make_pair(name, loc));
      std::cerr << "==smw> " << __PRETTY_FUNCTION__
                << " attribute: "
                << name
                << " type=" << type
                << " dim=" << dimension
                << " loc=" << loc
                << "\n";
    }
  }
}


void NoDice::OpenGLShaderPipeline::
activate_p()
{
  glUseProgram(this->program);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glUseProgram()");
}


bool NoDice::OpenGLShaderPipeline::
is_pipeline_active() const
{
  GLuint current_program;
  glGetIntegerv(GL_CURRENT_PROGRAM, reinterpret_cast<GLint*>(&current_program));
  return this->program == current_program;
}


void NoDice::OpenGLShaderPipeline::
deactivate_p()
{
  glUseProgram(0);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glUseProgram()");
}


void NoDice::OpenGLShaderPipeline::
set_uniform_p(const std::string& name, const NoDice::mat4& mat)
{
  auto it = this->uniform_index_map.find(name);
  if (it == this->uniform_index_map.end())
  {
    std::cerr << "WARNING: uniform \"" << name << "\" not found.\n";
    return;
  }
  glUniformMatrix4fv(it->second, 1, GL_FALSE, mat.array);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glUniformMatrix4fv()");
}


void NoDice::OpenGLShaderPipeline::
set_attribute_p(const std::string& name, int size, int stride, void const* ptr)
{
  auto it = this->attribute_index_map.find(name);
  if (it == this->attribute_index_map.end())
  {
    std::cerr << "WARNING: attribute \"" << name << "\" not found.\n";
    return;
  }
  glEnableVertexAttribArray(it->second);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glEnableVertexAttribArray()");
  glVertexAttribPointer(it->second, size, GL_FLOAT, GL_FALSE, stride, ptr);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glVertexAttribPointer()");
}

