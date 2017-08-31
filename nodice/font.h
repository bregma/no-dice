/**
 * @file nodice/font.h
 * @brief Public interface of the nodice/font module.
 *
 * Copyright 2009, 2010, 2017 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
#ifndef NODICE_FONT_H
#define NODICE_FONT_H 1

#include <cstdint>
#include "nodice/texture.h"
#include <string>
#include <vector>


namespace NoDice
{
  class App;

  struct Glyph
  {
    typedef std::vector<uint8_t> Bitmap;

    Bitmap    bitmap;  // bitmap
    int16_t   left;    // offset of glyph left from glyph origin
    int16_t   top;     // offset of glyph top from glyph origin
    uint16_t  width;   // width of bitmap in pixels
    uint16_t  height;  // height of bitmap in pixels
    int16_t   advance; // horizontal advance in pixels

    float     s;       // normalized X-offset of glyph within texture
    float     t;       // normalized Y-offset of glyph within texture
    float     w;       // normalized width of glyph within texture
    float     h;       // normalized height of glyph within texture
  };

  class Font
  {
  public:
    Font(App* app, std::string const& fontname, uint16_t height);
    ~Font();

    void
    mapToTexture(std::size_t texture_width_in_texels, std::size_t texture_height_in_texels);

    uint16_t
    height() const;

    // temporary function for testing
    void
    print(float x, float y, float scale, std::string const& text);

  private:
    App*               app;
    std::string        typeface;
    uint16_t           pointsize;
    std::vector<Glyph> glyph_metrics;
    Texture::OwningPtr texture;
  };
} // namespace NoDice

#endif // NODICE_FONT_H
