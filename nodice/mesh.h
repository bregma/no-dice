/**
 * @file mesh.h
 * @brief Public interface of the no-dice mesh module.
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
#ifndef NODICE_MESH_H
#define NODICE_MESH_H 1

#include <cstdint>
#include <memory>
#include <vector>


namespace NoDice
{
  /**
   * A collection of basic vertex and primitive information.
   *
   * A mesh can be assembled into a node in the scene graph.
   */
  class Mesh
  {
  public:
    /**
     * A mesh can contain all kinds of information about its vertexes, as
     * enumerated by the vertex target type.  The only mandatory target is the
     * position, if the mesh is to be drawable.
     *
     * @todo allow for instancing.
     */
    enum class VertexTargetType
    {
      Position, Normal, Color, Texcoord,
    };

    enum class VertexUsageHint
    {
      Static, Dynamic
    };

    struct VertexDescriptor
    {
      VertexTargetType target_type;
      std::uint16_t    rank;          ///< eg. vec2 == 2, vec3 == 3, mat4 == 16 etc.
      VertexUsageHint  usage_hint;
    };

    using VertexDescriptorList = std::vector<VertexDescriptor>;

    using OwningPtr = std::unique_ptr<Mesh>;

  public:
    Mesh();

    virtual
    ~Mesh() = 0;

    void
    add_vertex_data(std::size_t vertex_count, VertexDescriptorList const& desc, float const* data);

    void
    add_index_data(std::size_t index_count, std::uint16_t const* data);

    void
    draw();

  private:
    Mesh(Mesh const&) = delete;
    Mesh& operator=(Mesh const&) = delete;

    bool
    has_position_data() const;

    virtual void
    store_vertex_data(float const* data) = 0;

    virtual void
    reset_vertex_data() = 0;

    virtual void
    store_index_data(std::uint16_t const* data) = 0;

    virtual void
    reset_index_data() = 0;

    virtual void
    draw_indexed() = 0;

    virtual void
    draw_direct() = 0;

  protected:
    struct ArrayOffset
    {
      VertexTargetType target_type;
      std::size_t      offset_bytes;
    };
    using ArrayOffsets = std::vector<ArrayOffset>;

    std::size_t  vertex_stride_bytes_;
    ArrayOffsets vertex_offsets_;
    std::size_t  index_count_;
    std::size_t  vertex_count_;
  };
} // namespace NoDice

#endif // NODICE_MESH_H
