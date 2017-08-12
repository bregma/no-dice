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

#include <unordered_set>
#include <memory>
#include <string>


namespace NoDice
{
  class ShaderStage;
  class ShaderPipeline;

  class ShaderCache
  {
  public:
    /** Constructs am empty shader cache. */
    ShaderCache();

    /** Destructs a shader cache. */
    ~ShaderCache();

  private:
    ShaderCache(ShaderCache const&) = delete;
    ShaderCache& operator=(ShaderCache const&) = delete;

  private:
    using StageCache = std::unordered_set<std::unique_ptr<ShaderStage>>;
    using PipelineCache = std::unordered_set<std::unique_ptr<ShaderPipeline>>;
  };

} // namespace NoDice

#endif // NODICE_SHADERCACHE_H
