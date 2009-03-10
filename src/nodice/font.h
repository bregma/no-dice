/**
 * @file nodice/font.h
 * @brief Public interface of the nodice/font module.
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
#ifndef NODICE_FONT_H
#define NODICE_FONT_H 1

#include "opengl.h"
#include <string>


namespace NoDice
{
	class Font
	{
	public:
		Font(const std::string& fontname, unsigned int height);
		~Font();

	private:
		float   m_height;
		GLuint* m_textures;
	};
} // namespace NoDice

#endif // NODICE_FONT_H
