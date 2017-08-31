/**
 * @file texture.cpp
 * @brief Implemntation of the No-Dice texture module.
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
#include "nodice/texture.h"

#include <cassert>
#include "nodice/shaderpipeline.h"


NoDice::Texture::
~Texture()
{ }


void NoDice::Texture::
add_data(Target          target,
         int             lod_level,
         InternalFormat  internal_format,
         std::size_t     width_in_texels,
         std::size_t     height_in_texels,
         int             border,
         InputFormat     input_format,
         InputType       input_type,
         void const*     data)
{
  this->make_active();
  this->add_data_p(target, lod_level, internal_format, width_in_texels, height_in_texels, border, input_format, input_type, data);
  this->make_inactive();
}


void NoDice::Texture::
make_active()
{
  this->activate();
}


bool NoDice::Texture::
is_active()
{
  return is_active_p();
}


void NoDice::Texture::
make_inactive()
{
  this->deactivate();
}


void NoDice::Texture::
add_to_pipeline(ShaderPipeline& pipeline)
{
  assert(this->is_active());
  assert(pipeline.is_active());

  this->add_to_pipeline_p();
}

