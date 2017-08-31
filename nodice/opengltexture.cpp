/**
 * @file opengltexture.cpp
 * @brief Implemntation of the No-Dice OpenGL texture module.
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
#include "nodice/opengltexture.h"

#include <cassert>


namespace
{
  GLenum
  target_to_opengl(NoDice::OpenGLTexture::Target target)
  {
    static struct {
      NoDice::OpenGLTexture::Target target;
      GLenum                        value;
    } mapping[] = {
      { NoDice::OpenGLTexture::Target::Texture_2D, GL_TEXTURE_2D }
    };

    for (auto const& map: mapping)
    {
      if (target == map.target)
      {
        return map.value;
      }
    }
    return GL_TEXTURE_2D;
  }

  GLint
  internal_format_to_opengl(NoDice::OpenGLTexture::InternalFormat internal_format)
  {
    static struct {
      NoDice::OpenGLTexture::InternalFormat internal_format;
      GLint                                 value;
    } mapping[] = {
      { NoDice::OpenGLTexture::InternalFormat::Red8,                     GL_R8    },
      { NoDice::OpenGLTexture::InternalFormat::Red8_Green8_Blue8_Alpha8, GL_RGBA8 },
    };

    for (auto const& map: mapping)
    {
      if (internal_format == map.internal_format)
      {
        return map.value;
      }
    }
    return GL_RGBA;
  }

  GLenum
  input_format_to_opengl(NoDice::OpenGLTexture::InputFormat input_format)
  {
    static struct {
      NoDice::OpenGLTexture::InputFormat input_format;
      GLenum                             value;
    } mapping[] = {
      { NoDice::OpenGLTexture::InputFormat::Red,                   GL_RED             },
      { NoDice::OpenGLTexture::InputFormat::Red_Green,             GL_RG              },
      { NoDice::OpenGLTexture::InputFormat::Red_Green_Blue,        GL_RGB             },
      { NoDice::OpenGLTexture::InputFormat::Red_Green_Blue_Alpha,  GL_RGBA            },
      { NoDice::OpenGLTexture::InputFormat::Depth_Component,       GL_DEPTH_COMPONENT },
      { NoDice::OpenGLTexture::InputFormat::Depth_Stencil,         GL_DEPTH_STENCIL   },
    };

    for (auto const& map: mapping)
    {
      if (input_format == map.input_format)
      {
        return map.value;
      }
    }
    return GL_RGBA;
  }

  GLenum
  input_type_to_opengl(NoDice::OpenGLTexture::InputType input_type)
  {
    static struct {
      NoDice::OpenGLTexture::InputType input_type;
      GLenum                           value;
    } mapping[] = {
      { NoDice::OpenGLTexture::InputType::Byte,  GL_UNSIGNED_BYTE },
      { NoDice::OpenGLTexture::InputType::Float, GL_FLOAT         },
    };

    for (auto const& map: mapping)
    {
      if (input_type == map.input_type)
      {
        return map.value;
      }
    }
    return GL_UNSIGNED_BYTE;
  }

} // anonymous namespace


NoDice::OpenGLTexture::
OpenGLTexture()
{
  glGenTextures(1, &this->texture);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glGenTextures()");
}


NoDice::OpenGLTexture::
~OpenGLTexture()
{
  glDeleteTextures(1, &this->texture);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glDeleteTextures()");
}


void NoDice::OpenGLTexture::
add_data_p(NoDice::OpenGLTexture::Target          target,
           int                                    lod_level,
           NoDice::OpenGLTexture::InternalFormat  internal_format,
           std::size_t                            width_in_texels,
           std::size_t                            height_in_texels,
           int                                    border,
           NoDice::OpenGLTexture::InputFormat     input_format,
           NoDice::OpenGLTexture::InputType       input_type,
           void const*                            data)
{
  assert(this->is_active());

  glTexImage2D(target_to_opengl(target),
               lod_level,
               internal_format_to_opengl(internal_format),
               width_in_texels,
               height_in_texels,
               border,
               input_format_to_opengl(input_format),
               input_type_to_opengl(input_type),
               data);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glTexImage2D()");
}


void NoDice::OpenGLTexture::
activate()
{
  glBindTexture(GL_TEXTURE_2D, this->texture);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindTexture()");
}


bool NoDice::OpenGLTexture::
is_active_p() const
{
  GLuint current_texture;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&current_texture));
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glGetIntegerv(GL_TEXTURE_BINDING_2D)");

  return current_texture == this->texture;
}

void NoDice::OpenGLTexture::
deactivate()
{
  glBindTexture(GL_TEXTURE_2D, 0);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glBindTexture()");
}


void NoDice::OpenGLTexture::
add_to_pipeline_p()
{
  glActiveTexture(GL_TEXTURE0);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glActiveTexture()");
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glUniform1i(1, 0);
  check_gl_error(std::string{__PRETTY_FUNCTION__} + " glUniform1i()");
}
