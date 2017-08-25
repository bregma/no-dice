/**
 * @file mesh.cpp
 * @brief Implemntation of the No-Dice mesh module.
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
#include "nodice/mesh.h"

#include <cassert>
#include "nodice/shaderpipeline.h"


namespace
{

/**
 * Maps Mesh target types to shader attribute names.
 */
std::string
target_type_to_attribute_name(NoDice::Mesh::VertexTargetType target_type)
{
  static struct {
    NoDice::Mesh::VertexTargetType target_type;
    std::string                    attribute_name;
  } mapping[] = {
    { NoDice::Mesh::VertexTargetType::Position, "in_position" },
    { NoDice::Mesh::VertexTargetType::Normal,   "in_normal"   },
    { NoDice::Mesh::VertexTargetType::Color,    "in_color"    },
    { NoDice::Mesh::VertexTargetType::Texcoord, "in_texcoord" },
  };

  for (auto const& map: mapping)
  {
    if (target_type == map.target_type)
      return map.attribute_name;
  }
  return "unknown";
}

} // anonuymous namespace


NoDice::Mesh::
Mesh()
: index_count_(0)
, vertex_count_(0)
{ }


NoDice::Mesh::
~Mesh()
{ }


void NoDice::Mesh::
add_vertex_data(std::size_t vertex_count, std::vector<VertexDescriptor> const& desc, float const* data)
{
  if (this->vertex_count_ > 0)
    this->reset_vertex_data();

  this->vertex_stride_bytes_ = 0;
  for (auto const& d: desc)
  {
    this->vertex_offsets_.push_back({d.target_type, d.rank, this->vertex_stride_bytes_});
    this->vertex_stride_bytes_ += d.rank * sizeof(float);
  }
  this->vertex_count_ = vertex_count;
  this->store_vertex_data(data);
}


bool NoDice::Mesh::
has_position_data() const
{
  for (auto const& vo: this->vertex_offsets_)
  {
    if (vo.target_type == VertexTargetType::Position)
      return true;
  }
  return false;
}


void NoDice::Mesh::
add_index_data(std::size_t index_count, std::uint16_t const* data)
{
  if (this->index_count_ > 0)
    this->reset_index_data();

  this->index_count_ = index_count;
  this->store_index_data(data);
}


void NoDice::Mesh::
make_active()
{
  this->activate();
}


bool NoDice::Mesh::
is_active() const
{
  return this->is_active_p();
}


void NoDice::Mesh::
make_inactive()
{
  this->deactivate();
}


void NoDice::Mesh::
draw(NoDice::ShaderPipeline& pipeline)
{
  assert(this->has_position_data());
  assert(this->is_active());
  assert(pipeline.is_active());

  for (auto const& offset: this->vertex_offsets_)
  {
    std::string attribute_name = target_type_to_attribute_name(offset.target_type);
    pipeline.set_attribute(attribute_name,
                           offset.rank,
                           this->vertex_stride_bytes_,
                           static_cast<std::uint8_t*>(nullptr) + offset.offset_bytes);
  }

  if (this->index_count_ > 0)
  {
    this->draw_indexed();
  }
  else
  {
    this->draw_direct();
  }
}


