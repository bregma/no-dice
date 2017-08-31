/**
 * @file opengltexture.h
 * @brief Public interface of the No-Dice OpenGL texture module.
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
#ifndef NODICE_OPENGLTEXTURE_H
#define NODICE_OPENGLTEXTURE_H 1

#include "nodice/opengl.h"
#include "nodice/texture.h"


namespace NoDice
{

  class OpenGLTexture
  : public Texture
  {
  public:
    OpenGLTexture();

    ~OpenGLTexture();

  private:
    void
    add_data_p(Target          target,
               int             lod_level,
               InternalFormat  internal_format,
               std::size_t     width_in_texels,
               std::size_t     height_in_texels,
               int             border,
               InputFormat     input_format,
               InputType       input_type,
               void const*     data) override;

    void
    activate() override;

    bool
    is_active_p() const override;

    void
    deactivate() override;

    void
    add_to_pipeline_p() override;

  private:
    GLuint texture;
  };
} // namespace NoDice

#endif // NODICE_OPENGLTEXTURE_H
