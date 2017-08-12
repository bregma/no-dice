/**
 * @file nodice/shaderstage.h
 * @brief Public interface of the shader stage module.
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
#ifndef NODICE_SHADER_H
#define NODICE_SHADER_H 1

#include "nodice/opengl.h"
#include <string>


namespace NoDice
{
  class Config;

  class ShaderStage
  {
  public:
    /** Creates a ShaderStage. */
    ShaderStage(Config const&      config,
                GLenum             shader_type,
                std::string const& source_file_name);

    /** Destroys a ShaderStage. */
    ~ShaderStage();

    /** Gets the GL shader object. */
    GLint
    id() const;

  private:
    ShaderStage(ShaderStage const&) = delete;
    ShaderStage& operator=(ShaderStage const&) = delete;

  private:
    GLuint  shader_;
  };
}

#endif // NODICE_SHADER_H


