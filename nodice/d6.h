/**
 * @file nodice/d6.h
 * @brief Public interface of the nodice/d6 module.
 *
 * Copyright 2010 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
#ifndef NODICE_D6_H
#define NODICE_D6_H 1

#include "nodice/shape.h"
#include "nodice/video.h"


namespace NoDice
{

	class D6
	: public Shape
	{
	public:
		D6();
		~D6();
		void draw() const;

	private:
		GLuint  m_vbo;
		GLsizei m_vertexCount;
	};
} // namespace noDice

#endif // NODICE_D6_H
