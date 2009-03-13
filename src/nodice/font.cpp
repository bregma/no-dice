/**
 * @file nodice/font.cpp
 * @brief Implemntation of the nodice/font module.
 *
 * Copyright 2009 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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

	void check_gl_error(const std::string& msg)
	{
		GLenum err = glGetError();
		while (err != 0)
		{
			std::cerr << "GL error 0x" << std::hex << err << std::dec << " at " << msg << "\n";
			err = glGetError();
		}
	}
}


NoDice::Font::Font(const std::string& fontname, unsigned int pointsize)
: m_name(fontname)
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
				m_glyph[c].bitmap[i++] = ftFace->glyph->bitmap.buffer[index];
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
	GLfloat fTextureWidth = m_textureWidth;

	// Build a texture from the individual bitmaps.
	std::vector<GLubyte> texture(m_textureWidth
															 * m_textureHeight
															 * 2*sizeof(GLubyte));
	std::vector<GLubyte>::iterator it = texture.begin();
	GLsizei xoff = 0;
	for (unsigned char c = 0; c < s_max_char; ++c)
	{
		std::copy(m_glyph[c].bitmap.begin(), m_glyph[c].bitmap.end(), it);
		it += m_glyph[c].bitmap.size();
		xoff += m_glyph[c].width;

		m_glyph[c].s = static_cast<GLfloat>(xoff) / fTextureWidth;
		m_glyph[c].t = 0.0f;
		m_glyph[c].w = static_cast<GLfloat>(m_glyph[c].width) / fTextureWidth;
		m_glyph[c].h = 1.0f;
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
							 0, GL_RGBA, GL_UNSIGNED_BYTE,
							 &texture[0]);
	check_gl_error("glTexImage2D");
	glDisable(GL_TEXTURE_2D);
}


void NoDice::Font::print(GLfloat x, GLfloat y, const std::string& text)
{
	glPushAttrib(GL_TRANSFORM_BIT);
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glPopAttrib();
	glOrtho(viewport[0], viewport[2], viewport[1], viewport[3], -1.0f, 1.0f);

	glColor3ub(0, 0, 0xff);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  GLfloat varray[16];
	for (int i = 0; i < text.length(); ++i)
	{
		char c = text[i];
#if 0
		std::cerr << "==smw> c='" << c << "'"
		          << " x=" << x
		          << " y=" << y
			        << " s=" << m_glyph[c].s
			        << " t=" << m_glyph[c].t
			        << " s+w=" << m_glyph[c].s + m_glyph[c].w
			        << " t+h=" << m_glyph[c].t + m_glyph[c].h
			        << "\n";
#endif
		varray[0]  = x;
		varray[1]  = y;
		varray[2]  = m_glyph[c].s;
		varray[3]  = m_glyph[c].t + m_glyph[c].h;
		varray[4]  = x + m_glyph[c].width;
		varray[5]  = y;
		varray[6]  = m_glyph[c].s + m_glyph[c].w;
		varray[7]  = m_glyph[c].t + m_glyph[c].h;
		varray[8]  = x;
		varray[9]  = y + m_glyph[c].height;
		varray[10] = m_glyph[c].s;
		varray[11] = m_glyph[c].t;
		varray[12] = x + m_glyph[c].width;
		varray[13] = y + m_glyph[c].height;
		varray[14] = m_glyph[c].s + m_glyph[c].w;
		varray[15] = m_glyph[c].t;

		glVertexPointer(2, GL_FLOAT, 4*sizeof(GLfloat), varray);
		glTexCoordPointer(2, GL_FLOAT, 4*sizeof(GLfloat), ((char*)varray) + 2*sizeof(GLfloat));
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		check_gl_error("glDrawArrays");

		x += m_glyph[c].advance;
	}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}

