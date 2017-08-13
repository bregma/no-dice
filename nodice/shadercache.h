/**
 * @file shadercache.h
 * @brief Public interface of the shader cache module.
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
#ifndef NODICE_SHADERCACHE_H
#define NODICE_SHADERCACHE_H 1

#include "nodice/shaderpipeline.h"
#include "nodice/shaderstage.h"
#include <map>
#include <string>
#include <vector>


namespace NoDice
{
  class App;


  /**
   * The source of all things shader.
   *
   * Provides a shader pipeline given the application configuration and a list
   * of shader stages by name.  If the shader stages and pipeline are not
   * already in the cache, they get loaded from the appropriate shader resource
   * and set up appropriately.
   */
  class ShaderCache
  {
  public:
    /** Constructs am empty shader cache. */
    ShaderCache(App* app);

    /** Destructs a shader cache. */
    ~ShaderCache();

    /** A description of a shader stage to load. */
    struct StageDesc
    {
      ShaderStage::Type type;
      std::string       name;
    };

    /** Gets a (pointer to a) shader pipeline by shader stage name(s). */
    ShaderPipelinePtr
    get(std::vector<StageDesc> const& desc);

    /** Gets a (pointer to a) shader pipeline by ShaderId. */
    ShaderPipelinePtr
    get(ShaderPipeline::Id) const;

  private:
    ShaderCache(ShaderCache const&) = delete;
    ShaderCache& operator=(ShaderCache const&) = delete;

  private:
    using StageCache = std::map<ShaderStage::Id, ShaderStageOwningPtr>;
    using PipelineCache = std::map<ShaderPipeline::Id, ShaderPipelineOwningPtr>;

    App*           app_;
    StageCache     stage_cache_;
    PipelineCache  pipeline_cache_;
  };

} // namespace NoDice

#endif // NODICE_SHADERCACHE_H
