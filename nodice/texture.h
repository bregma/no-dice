/**
 * @file texture.h
 * @brief Public interface of the texture module.
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
#ifndef NODICE_TEXTURE_H
#define NODICE_TEXTURE_H 1

#include <cstddef>
#include <memory>


namespace NoDice
{
  class ShaderPipeline;

  /**
   * Abstract base class wrapping an in-memory texture.
   *
   * The actual implementation of the texture memory and handling is done in a
   * derived class through NVI.
   */
  class Texture
  {
  public:

    using OwningPtr = std::unique_ptr<Texture>;

    /**
     * The way to handle addressing outside of texture bounds.  The mode is set
     * separately for each axis of the texture.
     *
     * The default if otherwise unset is WrapMode::Repeat.
     */
    enum class WrapMode
    {
      ClampToEdge,
      MirroredRepeat,
      Repeat
    };

    /**
     * The way to interpolate the texels when zoomed in or out.
     */
    enum class FilterMode
    {
      Nearest,
      Linear
    };

    /**
     * Only 1 target texture type supported for now.
     * @todo support cube maps
     */
    enum class Target
    {
      Texture_2D
    };

    /**
     * @todo extend the supported formats.
     */
    enum class InputFormat
    {
      Red,
      Red_Green,
      Red_Green_Blue,
      Red_Green_Blue_Alpha,
      Depth_Component,
      Depth_Stencil
    };

    /**
     * @todo extend the supported types.
     */
    enum class InputType
    {
      Byte,
      Float
    };

    enum class InternalFormat
    {
      Red8,
      Red8_Green8_Blue8_Alpha8
    };

  public:
    virtual
    ~Texture() = 0;

    void
    add_data(Target          target,
             int             lod_level,
             InternalFormat  internal_format,
             std::size_t     width_in_texels,
             std::size_t     height_in_texels,
             int             border,
             InputFormat     input_format,
             InputType       input_type,
             void const*     data);

    void
    set_wrap_mode_u(WrapMode wrap_mode);

    void
    set_wrap_mode_v(WrapMode wrap_mode);

    void
    set_wrap_mode_w(WrapMode wrap_mode);

    void
    set_filter_mode_min(FilterMode filter_mode);

    void
    set_filter_mode_mag(FilterMode filter_mode);

    void
    make_active();

    bool
    is_active();

    void
    make_inactive();

    void
    add_to_pipeline(ShaderPipeline& pipeline);

  private:

    virtual void
    add_data_p(Target          target,
               int             lod_level,
               InternalFormat  internal_format,
               std::size_t     width_in_texels,
               std::size_t     height_in_texels,
               int             border,
               InputFormat     input_format,
               InputType       input_type,
               void const*     data) = 0;

    virtual void
    activate() = 0;

    virtual bool
    is_active_p() const = 0;

    virtual void
    deactivate() = 0;

    virtual void
    add_to_pipeline_p() = 0;
  };

} // namespace NoDice

#endif // NODICE_TEXTURE_H
