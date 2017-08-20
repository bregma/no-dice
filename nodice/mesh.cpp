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
    this->vertex_offsets_.push_back({d.target_type, this->vertex_stride_bytes_});
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
draw()
{
  assert(this->has_position_data());
  if (this->index_count_ > 0)
  {
    this->draw_indexed();
  }
  else
  {
    this->draw_direct();
  }
}


