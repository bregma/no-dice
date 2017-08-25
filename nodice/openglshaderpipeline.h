/**
 * @file openglshaderpipeline.h
 * @brief Public interface of the No-Dice OpenGL shader pipeline module.
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
#ifndef NODICE_OPENGLSHADERPIPELINE_H
#define NODICE_OPENGLSHADERPIPELINE_H 1

#include <map>
#include "nodice/opengl.h"
#include "nodice/shaderpipeline.h"
#include <string>


namespace NoDice
{

  class OpenGLShaderPipeline
  : public ShaderPipeline
  {
  public:
    OpenGLShaderPipeline(Id id);

    ~OpenGLShaderPipeline();

  private:
    using AttributeIndexMap = std::map<std::string, GLint>;
    using UniformIndexMap   = std::map<std::string, GLint>;

    void
    attach_stage_p(ShaderStage const& shader_stage) override;

    void
    link_p() override;

    void
    activate_p() override;

    bool
    is_pipeline_active() const override;

    void
    deactivate_p() override;

    void
    set_uniform_p(const std::string& name, const mat4& mat) override;

    void
    set_attribute_p(const std::string& name, int size, int stride, void const* ptr) override;

  private:
    GLuint            program;
    AttributeIndexMap attribute_index_map;
    UniformIndexMap   uniform_index_map;
  };

} // namespace NoDice

#endif // NODICE_OPENGLSHADERPIPELINE_H
