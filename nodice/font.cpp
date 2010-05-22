/**
 * @file nodice/font.cpp
 * @brief Implemntation of the nodice/font module.
 *
 * Copyright 2009, 2010 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of Version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include "nodice/font.h"

#include <algorithm>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>


namespace
{
	static const int s_max_char = 128;

	GLsizei nextPowerOfTwo(GLsizei x)
	{
		--x;
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		x |= x >> 8;
		x |= x >> 16;
		return ++x;
	}
}


NoDice::Font::Font(const std::string& fontname, unsigned int pointsize)
: m_name(fontname)
, m_height(pointsize)
, m_glyph(s_max_char)
{
	FT_Library ftLib;
	FT_Error ftStatus = FT_Init_FreeType(&ftLib);
	if (ftStatus != 0)
	{
		throw std::runtime_error("error in FT_Init_Freetype");
	}

	FT_Face ftFace;
	ftStatus = FT_New_Face(ftLib, m_name.c_str(), 0, &ftFace);
	if (ftStatus != 0)
	{
		throw std::runtime_error("error in FT_New_Face");
	}

	// munge character size.  Freetype uses 1/64th of a point (1/4608 of an inch)
	// as its base unit, but the pointsize parameter of this function is in
	// points.  Conversion requires multiplying by 64, which is what the
	// shift-by-size operation does.
	//
	// I'm also assuming a screen pitch of 100 dots-per-inch.  This needs to be
	// adjusted to use autodetected or configurable values if possible.
	// @todo fix assumptions about screen dot pitch
	//
	int dpi = 100;
	ftStatus = FT_Set_Char_Size(ftFace, pointsize<<6, pointsize<<6, dpi, dpi);

	// Import the bitmap for each character in the font.  This go-round, we're
	// only supportin 7-bit ASCII.
	// @todo fix assumptions about character sets
	GLsizei totalBitmapWidth = 0;
	GLsizei maxBitmapHeight = 0;
	for (unsigned char c = 0; c < s_max_char; ++c)
	{
		ftStatus = FT_Load_Char(ftFace, c, FT_LOAD_RENDER);
		if (ftStatus != 0)
		{
			throw std::runtime_error("error in FT_Load_Glyph");
		}

		m_glyph[c].left      = ftFace->glyph->bitmap_left;
		m_glyph[c].top       = ftFace->glyph->bitmap_top;
		m_glyph[c].width     = ftFace->glyph->bitmap.width;
		m_glyph[c].height    = ftFace->glyph->bitmap.rows;
		m_glyph[c].advance   = ftFace->glyph->advance.x >> 6;

		// Adjust running size totals
		totalBitmapWidth += m_glyph[c].width;
		maxBitmapHeight = std::max(maxBitmapHeight, m_glyph[c].height);

		// Allocate the required memory for this glyph's bitmap
		m_glyph[c].bitmap = Glyph::Bitmap(m_glyph[c].width
		                                  * m_glyph[c].height
		                                  * 2*sizeof(GLubyte));

		// Convert the freefont bitmap into an opengl GL_LUMINANCE_ALPHA bitmap
		int i = 0;
		for (int y = 0; y < m_glyph[c].height; ++y)
		{
			for (int x = 0; x < m_glyph[c].width; ++x)
			{
				int index = x + m_glyph[c].width*y;
				m_glyph[c].bitmap[i++] = ftFace->glyph->bitmap.buffer[index];
				m_glyph[c].bitmap[i++] = (ftFace->glyph->bitmap.buffer[index] > 0 ? 0xff : 0);
			}
		}
	}

	FT_Done_Face(ftFace);
	FT_Done_FreeType(ftLib);

	m_textureWidth  = nextPowerOfTwo(totalBitmapWidth);
	m_textureHeight = nextPowerOfTwo(maxBitmapHeight);
	mapToTexture();
}


NoDice::Font::~Font()
{
}


/**
 * Maps the font bitmaps to an OpenGL texture object.
 *
 * This is a public function so it can be called whenever the OpenGL context
 * gets destroyed (eg. after a windows resize on MS Windows).
 */
void NoDice::Font::mapToTexture()
{
	typedef std::vector<GLubyte> Texture;
	static const std::size_t bitmapDepth = 2*sizeof(GLubyte);
	const GLfloat fTextureWidth  = static_cast<GLfloat>(m_textureWidth);
	const GLfloat fTextureHeight = static_cast<GLfloat>(m_textureHeight);

	// Build a texture from the individual bitmaps.
	//
	// Start by allocating enough memory for the texture in
	// GL_LUMINANCE_ALPHA format, which requires 2 values for each pixel.
	// The data in the freetype bitmap are bytes, so each pixel in the
	// bitmap requires two bytes in the texture.
	//
	Texture texture(m_textureWidth * m_textureHeight * bitmapDepth);
	GLsizei texOffset = 0;
	for (unsigned char c = 0; c < s_max_char; ++c)
	{
		// Copy the glyph bitmap into the texture
		Texture::iterator  texLine  = texture.begin() + (texOffset * bitmapDepth);
		Texture::size_type texWidth = m_textureWidth * bitmapDepth;
		Glyph::Bitmap::size_type bitmapWidth = m_glyph[c].width * bitmapDepth;
		for (Glyph::Bitmap::const_iterator bitmapLine = m_glyph[c].bitmap.begin();
		     bitmapLine != m_glyph[c].bitmap.end();
		     bitmapLine += bitmapWidth, texLine += texWidth)
		{
			std::copy(bitmapLine, bitmapLine + bitmapWidth, texLine);
		}

		// Remember the texel coordinates for the glyph
		m_glyph[c].s = static_cast<GLfloat>(texOffset) / fTextureWidth;
		m_glyph[c].t = 0.0f;
		m_glyph[c].w = static_cast<GLfloat>(m_glyph[c].width) / fTextureWidth;
		m_glyph[c].h = static_cast<GLfloat>(m_glyph[c].height) / fTextureHeight;

		// Adjust the destination offset for the next glyph
		texOffset += m_glyph[c].width;
	}

	// Send it to the OpenGL engine.
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA,
	             m_textureWidth, m_textureHeight,
	             0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE,
	             &texture[0]);
	check_gl_error("glTexImage2D");
	glDisable(GL_TEXTURE_2D);
}


GLsizei NoDice::Font::height() const
{
	return m_height;
}


void NoDice::Font::print(GLfloat x, GLfloat y, GLfloat scale, const std::string& text)
{
	GLfloat viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
#ifdef HAVE_OPENGL_ES
	glOrthof(viewport[0], viewport[2], viewport[1], viewport[3], -1.0f, 1.0f);
#else
	glOrtho(viewport[0], viewport[2], viewport[1], viewport[3], -1.0f, 1.0f);
#endif

	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	static const int coords_per_vertex = 2;
	static const int coords_per_texture = 2;
	static const int row_width = coords_per_vertex + coords_per_texture;

	// Two coords per vertex, two coords per texture, three vertexes for first
	// triangle, one for second triangle = (2 + 2) * (3 + 1) = 16.
	GLfloat varray[16];
	for (std::string::const_iterator it = text.begin(); it != text.end(); ++it)
	{
		char c = *it;

		varray[0]  = x + m_glyph[c].left * scale;
		varray[1]  = y - (m_glyph[c].height - m_glyph[c].top) * scale;
		varray[2]  = m_glyph[c].s;
		varray[3]  = m_glyph[c].t + m_glyph[c].h;
		varray[4]  = x + m_glyph[c].width * scale;
		varray[5]  = y - (m_glyph[c].height - m_glyph[c].top) * scale;
		varray[6]  = m_glyph[c].s + m_glyph[c].w;
		varray[7]  = m_glyph[c].t + m_glyph[c].h;
		varray[8]  = x + m_glyph[c].left * scale;
		varray[9]  = y + m_glyph[c].top * scale;
		varray[10] = m_glyph[c].s;
		varray[11] = m_glyph[c].t;
		varray[12] = x + m_glyph[c].width * scale;
		varray[13] = y + m_glyph[c].top * scale;
		varray[14] = m_glyph[c].s + m_glyph[c].w;
		varray[15] = m_glyph[c].t;

		static const int stride = row_width * sizeof(varray[0]);

		glVertexPointer(coords_per_vertex, GL_FLOAT, stride, varray);
		glTexCoordPointer(coords_per_texture, GL_FLOAT, stride, ((char*)varray) + 2*sizeof(GLfloat));
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		check_gl_error("glDrawArrays");

		x += m_glyph[c].advance * scale;
	}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}


#ifndef DATA_DIR
# define DATA_DIR "./"
#endif

/**
 * @param typefaceName[in]  Name of the typeface.
 * @param pointSize[in]     Size of the font in points.
 *
 * Checks the global font cache for the named font and returns a reference to
 * it.  If the font is not in the cache, it gets loaded first.
 */
NoDice::Font& NoDice::
getFont(const std::string& typefaceName, unsigned int pointSize)
{
	typedef std::map<std::string, Font> FontCache;
	static FontCache s_fontCache;

	std::ostringstream ostr;
	ostr << typefaceName << '_' << pointSize;
	std::string fontKey = ostr.str();
	FontCache::iterator it = s_fontCache.find(fontKey);
	if (it != s_fontCache.end())
	{
		return  it->second;
	}
	ostr.str("");
	ostr << DATA_DIR << "assets/" << typefaceName << ".ttf";
	std::string fileName = ostr.str();
	std::pair<FontCache::iterator,bool> p = s_fontCache.insert(
											std::make_pair(fontKey, Font(fileName, pointSize)));
	return p.first->second;
}

