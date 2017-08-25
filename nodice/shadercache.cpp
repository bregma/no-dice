/**
 * @file shadercache.cpp
 * @brief Implemntation of the shader cache module.
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
#include "nodice/shadercache.h"

#include <iostream>
#include "nodice/app.h"
#include "nodice/config.h"
#include "nodice/openglshaderpipeline.h"
#include <stdexcept>


namespace
{
  /** Generates a uniquely identifying name for a pipeline from its stage names. */
  std::string
  stage_desc_to_pipeline_name(std::vector<NoDice::ShaderCache::StageDesc> const& stage_desc)
  {
    std::string pipeline_name;
    for (auto const d: stage_desc)
    {
      pipeline_name += ("/" + d.name);
    }
    return pipeline_name;
  }

  /** Generates a pipeline ID from its name. */
  NoDice::ShaderPipeline::Id
  pipeline_name_to_pipeline_id(std::string const& pipeline_name)
  {
    return std::hash<std::string>{}(pipeline_name);
  }
} // anonymous namespace


NoDice::ShaderCache::
ShaderCache(NoDice::App* app)
: app_(app)
{ }


NoDice::ShaderCache::
~ShaderCache()
{ }


NoDice::ShaderPipelinePtr NoDice::ShaderCache::
get(std::vector<NoDice::ShaderCache::StageDesc> const& stage_desc)
{
  ShaderPipelinePtr pipeline{};

  std::string pipeline_name{stage_desc_to_pipeline_name(stage_desc)};
  ShaderPipeline::Id pipeline_id{pipeline_name_to_pipeline_id(pipeline_name)};

  auto it = pipeline_cache_.find(pipeline_id);
  if (it == pipeline_cache_.end())
  {
    auto pipeline_pair = pipeline_cache_.emplace(pipeline_id,
                                                 ShaderPipelineOwningPtr{new OpenGLShaderPipeline(pipeline_id)});
    pipeline = pipeline_pair.first->second.get();

    for (auto const d: stage_desc)
    {
      ShaderStage::Id id = std::hash<std::string>{}(d.name);
      ShaderStagePtr stage = nullptr;
      auto it = stage_cache_.find(id);
      if (it == stage_cache_.end())
      {
        auto stage_pair = stage_cache_.emplace(id, std::make_unique<ShaderStage>(app_->config(), d.type, d.name));
        stage = stage_pair.first->second.get();
        if (app_->config().is_debug_mode())
          std::cerr << "==smw> " << __PRETTY_FUNCTION__ << "(): stage " << d.name << " added.\n";
      }
      else
      {
        stage = it->second.get();
      }
      pipeline->attach(*stage);
    }
    if (app_->config().is_debug_mode())
      std::cerr << "==smw> " << __PRETTY_FUNCTION__ << "(): pipeline " << pipeline_name << " added.\n";
  }
  else
  {
    pipeline = it->second.get();
  }

  return pipeline;
}


NoDice::ShaderPipelinePtr NoDice::ShaderCache::
get(ShaderPipeline::Id id) const
{
  auto it = pipeline_cache_.find(id);
  if (it != pipeline_cache_.end())
  {
    throw std::out_of_range("invalid shader pipeline ID");
  }
  return it->second.get();
}

