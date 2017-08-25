/**
 * @file nodice/font.cpp
 * @brief Implemntation of the nodice/font module.
 *
 * Copyright 2009-2013, 2017 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
#include "nodice_config.h"
#include "nodice/font.h"

#include <algorithm>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <iostream>
#include <map>
#include "nodice/maths.h"
#include "nodice/app.h"
#include "nodice/mesh.h"
#include "nodice/shadercache.h"
#include "nodice/shaderpipeline.h"
#include "nodice/shaderstage.h"
#include <sstream>
#include <stdexcept>
#include <unistd.h>


namespace
{
  constexpr int s_max_char = 128;

  uint16_t nextPowerOfTwo(uint16_t x)
  {
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return ++x;
  }
} // anonymous namespace


NoDice::Font::
Font(App* app, std::string const& typeface, uint16_t pointsize)
: app_(app)
, typeface_(typeface)
, pointsize_(pointsize)
, glyph_metrics_(s_max_char)
{
  // Calculate the maximum extent of textures supported by OpenGL.
  GLint max_tex_width;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_width);

  FT_Library ftLib;
  FT_Error ftStatus = FT_Init_FreeType(&ftLib);
  if (ftStatus != 0)
  {
    throw std::runtime_error("error in FT_Init_Freetype");
  }

  FT_Face ftFace;
  ftStatus = FT_New_Face(ftLib, typeface_.c_str(), 0, &ftFace);
  if (ftStatus != 0)
  {
                std::ostringstream ostr;
    ostr << "error " << ftStatus << " in FT_New_Face(\"" << typeface_ << "\"";
    throw std::runtime_error(ostr.str());
  }

  // munge character size.  Freetype uses 1/64th of a point (1/4608 of an inch)
  // as its base unit, but the pointsize parameter of this function is in
  // points.  Conversion requires multiplying by 64, which is what the
  // shift-left-by-six operation does.
  //
  // I'm also assuming a screen pitch of 100 dots-per-inch.  This needs to be
  // adjusted to use autodetected or configurable values if possible.
  // @todo fix assumptions about screen dot pitch
  //
  int dpi = 100;
  ftStatus = FT_Set_Char_Size(ftFace, pointsize_<<6, pointsize_<<6, dpi, dpi);

  // Import the bitmap for each character in the font.  This go-round, we're
  // only supporting 7-bit ASCII.
  // @todo fix assumptions about character sets
  uint16_t curTexWidth = 0;
  uint16_t maxTexWidth = 0;
  uint16_t curTexHeight = 0;
  uint16_t maxTexHeight = 0;
  int    glyphCount = 0;
  for (unsigned char c = 0; c < s_max_char; ++c)
  {
    ftStatus = FT_Load_Char(ftFace, c, FT_LOAD_RENDER);
    if (ftStatus != 0)
    {
      throw std::runtime_error("error in FT_Load_Glyph");
    }

    glyph_metrics_[c].left     = ftFace->glyph->bitmap_left;
    glyph_metrics_[c].top      = ftFace->glyph->bitmap_top;
    glyph_metrics_[c].width    = ftFace->glyph->bitmap.width;
    glyph_metrics_[c].height   = ftFace->glyph->bitmap.rows;
    glyph_metrics_[c].advance  = ftFace->glyph->advance.x >> 6;

    // If this glyph fits in current line, add to totals otherwise start a new
    // line.
    if (curTexWidth + glyph_metrics_[c].width < max_tex_width)
    {
      curTexWidth += glyph_metrics_[c].width;
      curTexHeight = std::max(curTexHeight, glyph_metrics_[c].height);
      ++glyphCount;
    }
    else
    {
      maxTexWidth = std::max(maxTexWidth, curTexWidth);
      curTexWidth = glyph_metrics_[c].width;
      maxTexHeight += std::max(curTexHeight, glyph_metrics_[c].height);
      curTexHeight = 0;
      glyphCount = 0;
    }

    // Allocate the required memory for this glyph's bitmap.
    //
    // The bitmap needs width*height bytes for the alpha component.
    glyph_metrics_[c].bitmap = Glyph::Bitmap(glyph_metrics_[c].width
                                      * glyph_metrics_[c].height
                                      * sizeof(GLubyte));

    // Convert the freefont bitmap into an alpha channel bitmap by
    // setting the alpha to 255 (0xff) wherever the luminance is greater than
    // zero.  See, a lot of truetype fonts have built-in antialiasing.  This
    // mechanism trats a zero luminance as a transparent background.
    int i = 0;
    for (int y = 0; y < glyph_metrics_[c].height; ++y)
    {
      for (int x = 0; x < glyph_metrics_[c].width; ++x)
      {
        int index = x + glyph_metrics_[c].width*y;
        glyph_metrics_[c].bitmap[i++] = ftFace->glyph->bitmap.buffer[index];
      }
    }
  }

  FT_Done_Face(ftFace);
  FT_Done_FreeType(ftLib);

  maxTexWidth = std::max(maxTexWidth, curTexWidth);
  maxTexHeight += curTexHeight;
  texture_width_in_texels_  = nextPowerOfTwo(maxTexWidth);
  texture_height_in_texels_ = nextPowerOfTwo(maxTexHeight);
  mapToTexture();
}


NoDice::Font::
~Font()
{
}


/**
 * Maps the font bitmaps to an OpenGL texture object.
 *
 * This is a public function so it can be called whenever the OpenGL context
 * gets destroyed (eg. after a windows resize on MS Windows).
 */
void NoDice::Font::
mapToTexture()
{
  typedef std::vector<uint8_t> Texture;

  constexpr std::size_t bitmapDepth = sizeof(uint8_t);

  // The stride for individual bitmap rows within the texture.
  const Texture::size_type stride = texture_width_in_texels_ * bitmapDepth;

  // Convenience variables to make later  conversion of tex coords onto (0,1)
  // easier.
  const float texture_width  = static_cast<float>(texture_width_in_texels_);
  const float texture_height = static_cast<float>(texture_height_in_texels_);

  Texture texture(texture_width_in_texels_ * texture_height_in_texels_ * bitmapDepth);

  // The current write-to position within the texture.  These values do not take
  // the bitmapDepth into account.
  unsigned int curTexX = 0;
  unsigned int curTexY = 0;

  for (unsigned char c = 0; c < s_max_char; ++c)
  {
    // Copy the glyph bitmap into the texture.
    //
    // The glyph bitmap is a packed array of height rows of width bytes.  This
    // has to be projected into the texture array as the same rows but but with
    // a stride of texture_width_in_texels_ * bitmapDepth bytes.
    //
    Texture::iterator texPos = texture.begin()
                             + curTexY * stride
                             + curTexX * bitmapDepth;
    Glyph::Bitmap::size_type bitmapWidth = glyph_metrics_[c].width * bitmapDepth;
    for (Glyph::Bitmap::const_iterator bitmapLine = glyph_metrics_[c].bitmap.begin();
         bitmapLine != glyph_metrics_[c].bitmap.end();
         bitmapLine += bitmapWidth, texPos += stride)
    {
      std::copy(bitmapLine, bitmapLine + bitmapWidth, texPos);
    }

    // Remember the texel coordinates for the glyph
    glyph_metrics_[c].s = static_cast<float>(curTexX) / texture_width;
    glyph_metrics_[c].t = static_cast<float>(curTexY) / texture_height;
    glyph_metrics_[c].w = static_cast<float>(glyph_metrics_[c].width) / texture_width;
    glyph_metrics_[c].h = static_cast<float>(glyph_metrics_[c].height) / texture_height;

    // Adjust the destination offset for the next glyph
    if (curTexX + glyph_metrics_[c].width < texture_width_in_texels_)
    {
      curTexX += glyph_metrics_[c].width;
    }
    else
    {
      curTexX = 0;
      ++curTexY;
    }
  }

  // Send it to the OpenGL engine.
  glGenTextures(1, &texture_id_);
  check_gl_error("Font::mapToTexture() glGenTextures()");
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  check_gl_error("Font::mapToTexture() glBindTexture()");
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
  check_gl_error("Font::mapToTexture() glTexParameteri(GL_TEXTURE_WRAP_S)");
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
  check_gl_error("Font::mapToTexture() glTexParameteri(GL_TEXTURE_WRAP_T)");
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  check_gl_error("Font::mapToTexture() glTexParameteri(GL_TEXTURE_MAG_FILTER)");
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  check_gl_error("Font::mapToTexture() glTexParameteri(GL_TEXTURE_MIN_FILTER)");
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  check_gl_error("Font::mapToTexture() glPixelStorei(GL_UNPACK_ALIGNMENT)");
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
               static_cast<GLsizei>(texture_width_in_texels_), static_cast<GLsizei>(texture_height_in_texels_),
               0, GL_RED, GL_UNSIGNED_BYTE,
               &texture[0]);
  check_gl_error("Font::mapToTexture() glTexImage2D");
}


uint16_t NoDice::Font::
height() const
{
  return pointsize_;
}


void NoDice::Font::
print(GLfloat x, GLfloat y, GLfloat scale, const std::string& text)
{
  constexpr int vertexes_per_glyph = 4;
  constexpr int indexes_per_glyph = 6;
  constexpr std::uint16_t index_order[] = { 0, 1, 2, 2, 1, 3 };

  // This will be an interleaved array of { X Y S T } values for each vertex.
  using VertexData = std::vector<vec4>;
  VertexData vertexes;
  vertexes.reserve(vertexes_per_glyph * text.length());

  // This is an index array for drawing two triangles for each glyph.
  using IndexData = std::vector<GLushort>;
  IndexData indexes;
  indexes.reserve(indexes_per_glyph * text.length());

  for (GLushort i = 0; i < text.length(); ++i)
  {
    auto const& c = text[i];
    auto const& glyph = glyph_metrics_[c];

    float left   = x + glyph.left * scale;
    float right  = x + glyph.width * scale;
    float top    = y - (glyph.height - glyph.top) * scale;
    float bottom = y + glyph.top * scale;

    vertexes.emplace_back(left,  top,    glyph.s,           glyph.t + glyph.h);
    vertexes.emplace_back(right, top,    glyph.s + glyph.w, glyph.t + glyph.h);
    vertexes.emplace_back(left,  bottom, glyph.s,           glyph.t);
    vertexes.emplace_back(right, bottom, glyph.s + glyph.w, glyph.t);

    std::uint16_t index_offset = i * vertexes_per_glyph;
    for (auto n: index_order)
      indexes.push_back(n + index_offset);

    x += glyph.advance * scale;
  }

  Mesh::OwningPtr mesh = app_->create_mesh();
  mesh->add_vertex_data(vertexes.size(),
                        {{Mesh::VertexTargetType::Position, 2, Mesh::VertexUsageHint::Static},
                         {Mesh::VertexTargetType::Texcoord, 2, Mesh::VertexUsageHint::Static}},
                        reinterpret_cast<float const*>(&vertexes[0]));
  mesh->add_index_data(indexes.size(), &indexes[0]);

  /* -- separation of mesh creation and rendering -- */

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  check_gl_error("Font::print() glGetIntegerv(GL_VIEWPORT)");
  vmml::Frustumf frustum((GLfloat)viewport[0], GLfloat(viewport[2]), GLfloat(viewport[1]), GLfloat(viewport[3]), -1.0f, 1.0f);
  mat4 MVP_matrix = frustum.computeOrthoMatrix();

  ShaderPipelinePtr shader_pipeline = app_->shader_cache().get({
                                        {ShaderStage::Type::Vertex,   "intro-vertex.glsl"},
                                        {ShaderStage::Type::Fragment, "intro-fragment.glsl"}
                                      });
  shader_pipeline->activate();
  shader_pipeline->set_uniform("mvp", MVP_matrix);

  /** @todo move into ShaderPipeline... */
  glActiveTexture(GL_TEXTURE0);
  check_gl_error("Font::print() glActiveTexture()");
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  check_gl_error("Font::print() glBindTexture()");
  glUniform1i(1, 0);
  check_gl_error("Font::print() glUniform1i()");
  /** ... */

  glEnable(GL_BLEND);
  check_gl_error("Font::print() glEnable(GL_BLEND)");
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  check_gl_error("Font::print() glBlendFunc()");

  mesh->make_active();
  mesh->draw(*shader_pipeline);
  mesh->make_inactive();
  shader_pipeline->deactivate();
}


