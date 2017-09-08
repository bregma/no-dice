/**
 * @file renderable.h
 * @brief Public interface of the No_dice renderable submodule.
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
#ifndef NODICE_RENDERABLE_H
#define NODICE_RENDERABLE_H 1

#include "nodice/maths.h"
#include <cstdint>


namespace NoDice
{
  class Mesh;
  class ShaderPipeline;
  class Texture;

  /**
   * A base class for anything that can be rendered to the screen.
   *
   * Rendering requires at the very least the following.
   *
   *   - a Mesh of vertexes
   *   - a poition (the model matrix)
   *   - a pointer to a valid shader pipeline
   *
   * In addition, the following optional attributes will be used.
   *
   *   - texture(s)
   *   - material
   *
   * Because of the optional nature of most attributes of a Renderable, it uses
   * piecewise construction.
   */
  class Renderable
  {
  public:
    /** Let the caller decide who owns a pointer? */
    enum class PointerOwnership
    {
      Owned,
      Unowned
    };

    static const std::uint16_t max_texture_count = 2;

  public:
    ~Renderable();

    void
    add_mesh(Mesh* mesh, PointerOwnership mesh_ownership = PointerOwnership::Unowned);

    void
    set_model_matrix(mat4 const& model_matrix)
    { this->model_matrix = model_matrix; }

    mat4 const&
    get_model_matrix() const
    { return this->model_matrix; }

    void
    set_shader_pipeline(ShaderPipeline* shader_pipeline)
    { this->shader_pipeline = shader_pipeline; }

    ShaderPipeline*
    get_shader_pipeline() const
    { return this->shader_pipeline; }

    void
    add_texture(std::uint16_t index, Texture* texture);

    std::uint16_t
    get_texture_count() const
    { return this->texture_count; }

    Texture*
    get_texture(std::uint16_t index);

    void
    draw() const;

  private:
    mat4             model_matrix                = mat4::IDENTITY;
    Mesh*            mesh                        = nullptr;
    PointerOwnership mesh_ownership              = PointerOwnership::Unowned;
    ShaderPipeline*  shader_pipeline             = nullptr;
    Texture*         textures[max_texture_count] = { nullptr };
    std::uint16_t    texture_count               = 0;
  };

} // namesspace NoDice


#endif // NODICE_RENDERABLE_H
