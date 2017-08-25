/**
 * @file nodice/shaderpipeline.cpp
 * @brief Private implemntation of the shader pipeline module.
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
#include "nodice/shaderpipeline.h"

#include <string>


/**
 * Constructs a shader pipeline object.
 */
NoDice::ShaderPipeline::
ShaderPipeline(NoDice::ShaderPipeline::Id id)
: pipeline_id(id)
, is_linked(false)
{ }


/**
 * Destroys the shader pipeline object.
 */
NoDice::ShaderPipeline::
~ShaderPipeline()
{ }


/**
 * @todo make this real.
 */
std::size_t NoDice::ShaderPipeline::
id() const
{ return this->pipeline_id; }


/**
 * Attaches a shader stage to the pipeline.
 */
void NoDice::ShaderPipeline::
attach(NoDice::ShaderStage const& shader_stage)
{
  attach_stage_p(shader_stage);
  this->is_linked = false;
}


/**
 * Links the stages in the pipeline.
 */
void NoDice::ShaderPipeline::
link()
{
  this->link_p();
  this->is_linked = true;
}


/**
 * Makes the pipeline the currect shader pipeline in use.
 */
void NoDice::ShaderPipeline::
activate()
{
  if (!this->is_linked)
  {
    link();
  }
  this->activate_p();
}


bool NoDice::ShaderPipeline::
is_active() const
{
  return this->is_pipeline_active();
}


void NoDice::ShaderPipeline::
deactivate()
{
  this->deactivate_p();
}


#if 0
void NoDice::ShaderPipeline::
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


void NoDice::ShaderPipeline::
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
#endif


void NoDice::ShaderPipeline::
set_uniform(const std::string& name, const NoDice::mat4& mat)
{
  this->set_uniform_p(name, mat);
}


void NoDice::ShaderPipeline::
set_attribute(const std::string& name, int size, int stride, void const* ptr)
{
  this->set_attribute_p(name, size, stride, ptr);
}

