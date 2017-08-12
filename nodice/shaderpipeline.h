/**
 * @file nodice/shaderpipeline.h
 * @brief Public interface of the shader pipeline module.
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
#ifndef NODICE_SHADERPIPELINE_H
#define NODICE_SHADERPIPELINE_H 1

#include <memory>
#include "nodice/maths.h"
#include <string>


namespace NoDice
{
  class ShaderStage;

  class ShaderPipeline
  {
  public:
    /** Creates a ShaderPipeline. */
    ShaderPipeline();

    /** Destroys a ShaderPipeline. */
    ~ShaderPipeline();

    /** Attaches a shader to the program. */
    void
    attach(ShaderStage const& shader_stage);

    /** Links the shader program (the final step in creation). */
    void
    link();

    /** Makes this shader program the active one in the current context. */
    void
    activate();

    /** Deactivates this shader program within the current context. */
    void
    deactivate();

    /** Sets a scalar uniform value. */
    void
    set_uniform(const std::string& name, float value);

    /** Sets a (vec3) uniform value. */
    void
    set_uniform(const std::string& name, float v1, float v2, float v3);

    /** Sets a mat4 uniform value. */
    void
    set_uniform(const std::string& name, const mat4& mat);

    /** Sets an attribute value. */
    void
    set_attribute(const std::string& name, int size, int stride, void const* ptr);

  private:
    ShaderPipeline(ShaderPipeline const&) = delete;
    ShaderPipeline& operator=(ShaderPipeline const&) = delete;

  private:
    class Impl;

    std::unique_ptr<Impl> impl_;
  };

  using ShaderPtr = ShaderPipeline*;
}

#endif // NODICE_SHADERPIPELINE_H


