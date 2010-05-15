/**
 * @file nodice/d6.cpp
 * @brief Implemntation of the nodice/d6 module.
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
#include "nodice/d6.h"

#include "nodice/maths.h"


namespace
{
	static const int row_width = NoDice::coords_per_vertex
	                           + NoDice::coords_per_normal;
} // anonymous namespace

NoDice::D6::
D6()
: Shape("d6", NoDice::Colour(1.0f, 0.0f, 0.1f, 0.40f))
{
	static const GLfloat bevel = 0.05f;
	static const GLfloat size = 1.0f / std::sqrt(2.0f);
	static const GLfloat bsize = size - bevel;

	/* vertex-3, normal-3 */
	GLfloat cube[] = {
		/* right side */
		 size,  -bsize,   bsize,  1.0f,  0.0f,  0.0f, /* A */
		 size,  -bsize,  -bsize,  1.0f,  0.0f,  0.0f, /* B */
		 size,   bsize,   bsize,  1.0f,  0.0f,  0.0f, /* C */
		 size,   bsize,   bsize,  1.0f,  0.0f,  0.0f, /* C */
		 size,  -bsize,  -bsize,  1.0f,  0.0f,  0.0f, /* B */
		 size,   bsize,  -bsize,  1.0f,  0.0f,  0.0f, /* D */
		/* right top bevel */
		 size,   bsize,   bsize,  1.0f,  1.0f,  0.0f, /* C */
		 size,   bsize,  -bsize,  1.0f,  1.0f,  0.0f, /* D */
		 bsize,   size,   bsize,  1.0f,  1.0f,  0.0f, /* E */
		 bsize,   size,   bsize,  1.0f,  1.0f,  0.0f, /* E */
		 size,   bsize,  -bsize,  1.0f,  1.0f,  0.0f, /* D */
		 bsize,   size,  -bsize,  1.0f,  1.0f,  0.0f, /* F */
		/* top side */
		 bsize,   size,   bsize,  0.0f,  1.0f,  0.0f, /* E */
		 bsize,   size,  -bsize,  0.0f,  1.0f,  0.0f, /* F */
		-bsize,   size,   bsize,  0.0f,  1.0f,  0.0f, /* G */
		-bsize,   size,   bsize,  0.0f,  1.0f,  0.0f, /* G */
		 bsize,   size,  -bsize,  0.0f,  1.0f,  0.0f, /* F */
		-bsize,   size,  -bsize,  0.0f,  1.0f,  0.0f, /* H */
		/* left top bevel */
		-bsize,   size,   bsize, -1.0f,  1.0f,  0.0f, /* G */
		-bsize,   size,  -bsize, -1.0f,  1.0f,  0.0f, /* H */
		-size,   bsize,   bsize, -1.0f,  1.0f,  0.0f, /* I */
		-size,   bsize,   bsize, -1.0f,  1.0f,  0.0f, /* I */
		-bsize,   size,  -bsize, -1.0f,  1.0f,  0.0f, /* H */
		-size,   bsize,  -bsize, -1.0f,  1.0f,  0.0f, /* J */
		/* left side */
		-size,   bsize,   bsize, -1.0f,  0.0f,  0.0f, /* I */
		-size,   bsize,  -bsize, -1.0f,  0.0f,  0.0f, /* J */
		-size,  -bsize,   bsize, -1.0f,  0.0f,  0.0f, /* K */
		-size,  -bsize,   bsize, -1.0f,  0.0f,  0.0f, /* K */
		-size,   bsize,  -bsize, -1.0f,  0.0f,  0.0f, /* J */
		-size,  -bsize,  -bsize, -1.0f,  0.0f,  0.0f, /* L */
		/* left bottom bevel */
		-size,  -bsize,   bsize, -1.0f, -1.0f,  0.0f, /* K */
		-size,  -bsize,  -bsize, -1.0f, -1.0f,  0.0f, /* L */
		-bsize,  -size,   bsize, -1.0f, -1.0f,  0.0f, /* M */
		-bsize,  -size,   bsize, -1.0f, -1.0f,  0.0f, /* M */
		-size,  -bsize,  -bsize, -1.0f, -1.0f,  0.0f, /* L */
		-bsize,  -size,  -bsize, -1.0f, -1.0f,  0.0f, /* N */
		/* bottom */
		-bsize,  -size,   bsize,  0.0f, -1.0f,  0.0f, /* M */
		-bsize,  -size,  -bsize,  0.0f, -1.0f,  0.0f, /* N */
		 bsize,  -size,   bsize,  0.0f, -1.0f,  0.0f, /* O */
		 bsize,  -size,   bsize,  0.0f, -1.0f,  0.0f, /* O */
		-bsize,  -size,  -bsize,  0.0f, -1.0f,  0.0f, /* N */
		 bsize,  -size,  -bsize,  0.0f, -1.0f,  0.0f, /* P */
		/* right bottom bevel */
		 bsize,  -size,   bsize,  1.0f, -1.0f,  0.0f, /* O */
		 bsize,  -size,  -bsize,  1.0f, -1.0f,  0.0f, /* P */
		 size,  -bsize,   bsize,  1.0f, -1.0f,  0.0f, /* A */
		 size,  -bsize,   bsize,  1.0f, -1.0f,  0.0f, /* A */
		 bsize,  -size,  -bsize,  1.0f, -1.0f,  0.0f, /* P */
		 size,  -bsize,  -bsize,  1.0f, -1.0f,  0.0f, /* B */
		/* right front bevel */
		 size,  -bsize,   bsize,  1.0f,  0.0f,  1.0f, /* A */
		 size,   bsize,   bsize,  1.0f,  0.0f,  1.0f, /* C */
		 bsize, -bsize,  size,  1.0f,  0.0f,  1.0f, /* Q */
		 bsize, -bsize,  size,  1.0f,  0.0f,  1.0f, /* Q */
		 size,   bsize,   bsize,  1.0f,  0.0f,  1.0f, /* C */
		 bsize,  bsize,  size,  1.0f,  0.0f,  1.0f, /* R */
		/* right top front corner */
		 bsize,  bsize,  size,  1.0f,  1.0f,  1.0f, /* R */
		 size,   bsize,   bsize,  1.0f,  1.0f,  1.0f, /* C */
		 bsize,  size,   bsize,  1.0f,  1.0f,  1.0f, /* E */
		/* top front bevel */
		 bsize,  bsize,  size,  0.0f,  1.0f,  1.0f, /* R */
		 bsize,  size,   bsize,  0.0f,  1.0f,  1.0f, /* E */
		-bsize,  bsize,  size,  0.0f,  1.0f,  1.0f, /* S */
		-bsize,  bsize,  size,  0.0f,  1.0f,  1.0f, /* S */
		 bsize,  size,   bsize,  0.0f,  1.0f,  1.0f, /* E */
		-bsize,  size,   bsize,  0.0f,  1.0f,  1.0f, /* G */
		/* left top front corner */
		-bsize,  bsize,  size, -1.0f,  1.0f,  1.0f, /* S */
		-bsize,  size,   bsize, -1.0f,  1.0f,  1.0f, /* G */
		-size,   bsize,   bsize, -1.0f,  1.0f,  1.0f, /* I */
		/* left front bevel */
		-bsize,  bsize,  size, -1.0f,  0.0f,  1.0f, /* S */
		-size,   bsize,   bsize, -1.0f,  0.0f,  1.0f, /* I */
		-bsize, -bsize,  size, -1.0f,  0.0f,  1.0f, /* T */
		-bsize, -bsize,  size, -1.0f,  0.0f,  1.0f, /* T */
		-size,   bsize,   bsize, -1.0f,  0.0f,  1.0f, /* I */
		-size,  -bsize,   bsize, -1.0f,  0.0f,  1.0f, /* K */
		/* left bottom front corner */
		-bsize, -bsize,  size, -1.0f, -1.0f,  1.0f, /* T */
		-size,  -bsize,   bsize, -1.0f, -1.0f,  1.0f, /* K */
		-bsize, -size,   bsize, -1.0f, -1.0f,  1.0f, /* M */
		/* bottom front bevel */
		-bsize, -size,   bsize,  0.0f, -1.0f,  1.0f, /* M */
		 bsize, -size,   bsize,  0.0f, -1.0f,  1.0f, /* O */
		-bsize, -bsize,  size,  0.0f, -1.0f,  1.0f, /* T */
		-bsize, -bsize,  size,  0.0f, -1.0f,  1.0f, /* T */
		 bsize, -size,   bsize,  0.0f, -1.0f,  1.0f, /* O */
		 bsize, -bsize,  size,  0.0f, -1.0f,  1.0f, /* Q */
		/* right bottom front corner */
		 bsize, -bsize,  size,  1.0f, -1.0f,  1.0f, /* Q */
		 bsize, -size,   bsize,  1.0f, -1.0f,  1.0f, /* O */
		 size,  -bsize,   bsize,  1.0f, -1.0f,  1.0f, /* A */
		/* front */
		-bsize,   -bsize,  size,  0.0f,  0.0f,  1.0f, /* T */
		 bsize,   -bsize,  size,  0.0f,  0.0f,  1.0f, /* Q */
		 bsize,    bsize,  size,  0.0f,  0.0f,  1.0f, /* R */
		-bsize,   -bsize,  size,  0.0f,  0.0f,  1.0f, /* T */
		 bsize,    bsize,  size,  0.0f,  0.0f,  1.0f, /* R */
		-bsize,    bsize,  size,  0.0f,  0.0f,  1.0f, /* S */
		/* right back bevel */
		size,  -bsize,  -bsize,  1.0f,  0.0f, -1.0f, /* B */
		 bsize,  -bsize, -size,  1.0f,  0.0f, -1.0f, /* V */
		size,   bsize,  -bsize,  1.0f,  0.0f, -1.0f, /* D */
		size,   bsize,  -bsize,  1.0f,  0.0f, -1.0f, /* D */
		 bsize,  -bsize, -size,  1.0f,  0.0f, -1.0f, /* V */
		 bsize,   bsize, -size,  1.0f,  0.0f, -1.0f, /* U */
		/* right top back corner */
		 bsize,   bsize, -size,  1.0f,  1.0f, -1.0f, /* U */
		 bsize,  size,  -bsize,  1.0f,  1.0f, -1.0f, /* F */
		size,   bsize,  -bsize,  1.0f,  1.0f, -1.0f, /* D */
		/* top back bevel */
		 bsize,   bsize, -size,  0.0f,  1.0f, -1.0f, /* U */
		-bsize,   bsize, -size,  0.0f,  1.0f, -1.0f, /* X */
		 bsize,  size,  -bsize,  0.0f,  1.0f, -1.0f, /* F */
		 bsize,  size,  -bsize,  0.0f,  1.0f, -1.0f, /* F */
		-bsize,   bsize, -size,  0.0f,  1.0f, -1.0f, /* X */
		-bsize,  size,  -bsize,  0.0f,  1.0f, -1.0f, /* H */
		/* left top back corner */
		 -bsize,  size,  -bsize, -1.0f,  1.0f, -1.0f, /* H */
		 -bsize,   bsize, -size, -1.0f,  1.0f, -1.0f, /* X */
		-size,   bsize,  -bsize, -1.0f,  1.0f, -1.0f, /* J */
		/* left back bevel */
		 -bsize,  -bsize, -size, -1.0f,  0.0f, -1.0f, /* W */
		-size,  -bsize,  -bsize, -1.0f,  0.0f, -1.0f, /* L */
		 -bsize,   bsize, -size, -1.0f,  0.0f, -1.0f, /* X */
		 -bsize,   bsize, -size, -1.0f,  0.0f, -1.0f, /* X */
		-size,  -bsize,  -bsize, -1.0f,  0.0f, -1.0f, /* L */
		-size,   bsize,  -bsize, -1.0f,  0.0f, -1.0f, /* J */
		/* left bottom back corner */
		 -bsize,  -bsize, -size, -1.0f, -1.0f, -1.0f, /* W */
		 -bsize, -size,  -bsize, -1.0f, -1.0f, -1.0f, /* N */
		-size,  -bsize,  -bsize, -1.0f, -1.0f, -1.0f, /* L */
		/* bottom back bevel */
		  bsize, -size,  -bsize,  0.0f, -1.0f, -1.0f, /* P */
		 -bsize, -size,  -bsize,  0.0f, -1.0f, -1.0f, /* N */
		  bsize,  -bsize, -size,  0.0f, -1.0f, -1.0f, /* V */
		  bsize,  -bsize, -size,  0.0f, -1.0f, -1.0f, /* V */
		 -bsize, -size,  -bsize,  0.0f, -1.0f, -1.0f, /* N */
		 -bsize,  -bsize, -size,  0.0f, -1.0f, -1.0f, /* W */
		/* right bottom back corner */
		 size,  -bsize,  -bsize,  1.0f, -1.0f, -1.0f, /* B */
		 bsize, -size,  -bsize,  1.0f, -1.0f, -1.0f, /* P */
		 bsize,  -bsize, -size,  1.0f, -1.0f, -1.0f, /* V */
		/* back */
		 bsize,   -bsize, -size,  0.0f,  0.0f, -1.0f, /* V */
		-bsize,   -bsize, -size,  0.0f,  0.0f, -1.0f, /* W */
		 bsize,    bsize, -size,  0.0f,  0.0f, -1.0f, /* U */
		 bsize,    bsize, -size,  0.0f,  0.0f, -1.0f, /* U */
		-bsize,   -bsize, -size,  0.0f,  0.0f, -1.0f, /* W */
		-bsize,    bsize, -size,  0.0f,  0.0f, -1.0f, /* X */
	};
	m_vertexCount = (sizeof(cube) / sizeof(GLfloat)) / row_width;

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
} 


NoDice::D6::
~D6()
{
	glDeleteBuffers(1, &m_vbo);
} 


void NoDice::D6::
draw() const
{
 	static const int stride = row_width * sizeof(GLfloat);
 	static const GLfloat* cube_verteces = 0;
	static const GLfloat* cube_normals = cube_verteces + coords_per_vertex;

	glEnable(GL_NORMALIZE);
	glDisable(GL_CULL_FACE); // for transparency -- move to object
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glNormalPointer(GL_FLOAT, stride, cube_normals);
	glVertexPointer(3, GL_FLOAT, stride, cube_verteces);
	glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_NORMALIZE);
}

