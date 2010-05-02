/**
 * @file nodice/font.h
 * @brief Public interface of the nodice/font module.
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
#ifndef NODICE_FONT_H
#define NODICE_FONT_H 1

#include "opengl.h"
#include <string>
#include <vector>


namespace NoDice
{
	struct Glyph
	{
		typedef std::vector<GLubyte> Bitmap;

		Bitmap   bitmap;  // bitmap
		GLsizei  left;
		GLsizei  top;
		GLsizei  width;	  // width of bitmap in pixels
		GLsizei  height;  // height of bitmap in pixels
		GLsizei  advance; // horizontal advance in pixels

		GLfloat  s;       // X-offset of glyph within texture
		GLfloat  t;       // Y-offset of glyph within texture
		GLfloat  w;       // width of glyph within texture
		GLfloat  h;       // height of glyph within texture
	};

	class Font
	{
	public:
		Font(const std::string& fontname, unsigned int height);
		~Font();

		void mapToTexture();

		GLsizei height() const;

		// temporary function for testing
		void print(GLfloat x, GLfloat y, GLfloat scale, const std::string& text);

	private:
		std::string        m_name;
		float              m_height;
		std::vector<Glyph> m_glyph;

		GLsizei            m_textureWidth;
		GLsizei            m_textureHeight;
		GLuint             m_texture;
	};

	/** Gets a font through the font loader/cache. */
	Font& getFont(const std::string& fontName, unsigned int pointSize);
} // namespace NoDice

#endif // NODICE_FONT_H
