/**
 * @file renderable.cpp
 * @brief Implemntation of the No-Dice Renderable submodule.
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
#include "nodice/renderable.h"

#include <algorithm>
#include "nodice/mesh.h"
#include <stdexcept>


NoDice::Renderable::
~Renderable()
{
  if (this->mesh_ownership == PointerOwnership::Owned)
    delete this->mesh;
}


void NoDice::Renderable::
add_mesh(Mesh* mesh, PointerOwnership mesh_ownership)
{
  if (this->mesh_ownership == PointerOwnership::Owned)
    delete this->mesh;

  this->mesh = mesh;
  this->mesh_ownership = mesh_ownership;
}


void NoDice::Renderable::
add_texture(std::uint16_t index, Texture* texture)
{
  assert(index < this->max_texture_count);
  this->textures[index] = texture;
  this->texture_count = std::max(this->texture_count, index) + 1;
}


NoDice::Texture* NoDice::Renderable::
get_texture(std::uint16_t index)
{
  assert(index < this->max_texture_count);
  return this->textures[index];
}


void NoDice::Renderable::
draw() const
{
  if (this->mesh == nullptr)
    throw std::logic_error("no mesh in renderable");
  if (this->shader_pipeline == nullptr)
    throw std::logic_error("no shader pipeline in renderable");

  this->mesh->make_active();
  this->mesh->draw(*this->shader_pipeline);
  this->mesh->make_inactive();
}

