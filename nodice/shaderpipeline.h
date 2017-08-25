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
    /** Uniquely identifies this particular shader pipeline. */
    using Id = std::size_t;

  public:
    /** Creates a ShaderPipeline. */
    explicit
    ShaderPipeline(Id id);

    /** Destroys a ShaderPipeline. */
    virtual
    ~ShaderPipeline() = 0;

    Id
    id() const;

    /** Attaches a shader stage to the pipeline. */
    void
    attach(ShaderStage const& shader_stage);

    /** Links the shader pipeline (the final step in creation). */
    void
    link();

    /** Makes this shader pipeline the active one in the current context. */
    void
    activate();

    bool
    is_active() const;

    /** Deactivates this shader pipeline within the current context. */
    void
    deactivate();

#if 0
    /** Sets a scalar uniform value. */
    void
    set_uniform(const std::string& name, float value);

    /** Sets a (vec3) uniform value. */
    void
    set_uniform(const std::string& name, float v1, float v2, float v3);
#endif

    /** Sets a mat4 uniform value. */
    void
    set_uniform(const std::string& name, const mat4& mat);

    /** Sets an attribute value. */
    virtual void
    set_attribute(const std::string& name, int size, int stride, void const* ptr);

  private:
    ShaderPipeline(ShaderPipeline const&) = delete;
    ShaderPipeline& operator=(ShaderPipeline const&) = delete;

    virtual void
    attach_stage_p(ShaderStage const& shader_stage) = 0;

    virtual void
    link_p() = 0;

    virtual void
    activate_p() = 0;

    virtual bool
    is_pipeline_active() const = 0;

    virtual void
    deactivate_p() = 0;

    virtual void
    set_uniform_p(const std::string& name, const mat4& mat) = 0;

    virtual void
    set_attribute_p(const std::string& name, int size, int stride, void const* ptr) = 0;

  private:
    Id    pipeline_id;
    bool  is_linked;
  };

  using ShaderPipelineOwningPtr = std::unique_ptr<ShaderPipeline>;
  using ShaderPipelinePtr       = ShaderPipeline*;
}

#endif // NODICE_SHADERPIPELINE_H


