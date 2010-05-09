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


namespace
{
	static const int row_width = 3*2;
} // anonymous namespace

NoDice::D6::
D6()
: Shape("d6")
{
	const GLfloat sizex = 1.0f;
	const GLfloat sizey = 1.0f;
	const GLfloat sizez = 1.0f;
	const GLfloat bevel = 0.08f;
	const GLfloat bsizex = sizex + bevel;
	const GLfloat bsizey = sizey + bevel;
	const GLfloat bsizez = sizez + bevel;

	/* vertex-3, normal-3 */
	GLfloat cube[] = {
		/* right side */
		 bsizex,  -sizey,   sizez,  1.0f,  0.0f,  0.0f, /* A */
		 bsizex,  -sizey,  -sizez,  1.0f,  0.0f,  0.0f, /* B */
		 bsizex,   sizey,   sizez,  1.0f,  0.0f,  0.0f, /* C */
		 bsizex,   sizey,   sizez,  1.0f,  0.0f,  0.0f, /* C */
		 bsizex,  -sizey,  -sizez,  1.0f,  0.0f,  0.0f, /* B */
		 bsizex,   sizey,  -sizez,  1.0f,  0.0f,  0.0f, /* D */
		/* right top bevel */
		 bsizex,   sizey,   sizez,  1.0f,  1.0f,  0.0f, /* C */
		 bsizex,   sizey,  -sizez,  1.0f,  1.0f,  0.0f, /* D */
		 sizex,   bsizey,   sizez,  1.0f,  1.0f,  0.0f, /* E */
		 sizex,   bsizey,   sizez,  1.0f,  1.0f,  0.0f, /* E */
		 bsizex,   sizey,  -sizez,  1.0f,  1.0f,  0.0f, /* D */
		 sizex,   bsizey,  -sizez,  1.0f,  1.0f,  0.0f, /* F */
		/* top side */
		 sizex,   bsizey,   sizez,  0.0f,  1.0f,  0.0f, /* E */
		 sizex,   bsizey,  -sizez,  0.0f,  1.0f,  0.0f, /* F */
		-sizex,   bsizey,   sizez,  0.0f,  1.0f,  0.0f, /* G */
		-sizex,   bsizey,   sizez,  0.0f,  1.0f,  0.0f, /* G */
		 sizex,   bsizey,  -sizez,  0.0f,  1.0f,  0.0f, /* F */
		-sizex,   bsizey,  -sizez,  0.0f,  1.0f,  0.0f, /* H */
		/* left top bevel */
		-sizex,   bsizey,   sizez, -1.0f,  1.0f,  0.0f, /* G */
		-sizex,   bsizey,  -sizez, -1.0f,  1.0f,  0.0f, /* H */
		-bsizex,   sizey,   sizez, -1.0f,  1.0f,  0.0f, /* I */
		-bsizex,   sizey,   sizez, -1.0f,  1.0f,  0.0f, /* I */
		-sizex,   bsizey,  -sizez, -1.0f,  1.0f,  0.0f, /* H */
		-bsizex,   sizey,  -sizez, -1.0f,  1.0f,  0.0f, /* J */
		/* left side */
		-bsizex,   sizey,   sizez, -1.0f,  0.0f,  0.0f, /* I */
		-bsizex,   sizey,  -sizez, -1.0f,  0.0f,  0.0f, /* J */
		-bsizex,  -sizey,   sizez, -1.0f,  0.0f,  0.0f, /* K */
		-bsizex,  -sizey,   sizez, -1.0f,  0.0f,  0.0f, /* K */
		-bsizex,   sizey,  -sizez, -1.0f,  0.0f,  0.0f, /* J */
		-bsizex,  -sizey,  -sizez, -1.0f,  0.0f,  0.0f, /* L */
		/* left bottom bevel */
		-bsizex,  -sizey,   sizez, -1.0f, -1.0f,  0.0f, /* K */
		-bsizex,  -sizey,  -sizez, -1.0f, -1.0f,  0.0f, /* L */
		-sizex,  -bsizey,   sizez, -1.0f, -1.0f,  0.0f, /* M */
		-sizex,  -bsizey,   sizez, -1.0f, -1.0f,  0.0f, /* M */
		-bsizex,  -sizey,  -sizez, -1.0f, -1.0f,  0.0f, /* L */
		-sizex,  -bsizey,  -sizez, -1.0f, -1.0f,  0.0f, /* N */
		/* bottom */
		-sizex,  -bsizey,   sizez,  0.0f, -1.0f,  0.0f, /* M */
		-sizex,  -bsizey,  -sizez,  0.0f, -1.0f,  0.0f, /* N */
		 sizex,  -bsizey,   sizez,  0.0f, -1.0f,  0.0f, /* O */
		 sizex,  -bsizey,   sizez,  0.0f, -1.0f,  0.0f, /* O */
		-sizex,  -bsizey,  -sizez,  0.0f, -1.0f,  0.0f, /* N */
		 sizex,  -bsizey,  -sizez,  0.0f, -1.0f,  0.0f, /* P */
		/* right bottom bevel */
		 sizex,  -bsizey,   sizez,  1.0f, -1.0f,  0.0f, /* O */
		 sizex,  -bsizey,  -sizez,  1.0f, -1.0f,  0.0f, /* P */
		 bsizex,  -sizey,   sizez,  1.0f, -1.0f,  0.0f, /* A */
		 bsizex,  -sizey,   sizez,  1.0f, -1.0f,  0.0f, /* A */
		 sizex,  -bsizey,  -sizez,  1.0f, -1.0f,  0.0f, /* P */
		 bsizex,  -sizey,  -sizez,  1.0f, -1.0f,  0.0f, /* B */
		/* right front bevel */
		 bsizex,  -sizey,   sizez,  1.0f,  0.0f,  1.0f, /* A */
		 bsizex,   sizey,   sizez,  1.0f,  0.0f,  1.0f, /* C */
		 sizex,   -sizey,  bsizez,  1.0f,  0.0f,  1.0f, /* Q */
		 sizex,   -sizey,  bsizez,  1.0f,  0.0f,  1.0f, /* Q */
		 bsizex,   sizey,   sizez,  1.0f,  0.0f,  1.0f, /* C */
		 sizex,    sizey,  bsizez,  1.0f,  0.0f,  1.0f, /* R */
		/* right top front corner */
		 sizex,    sizey,  bsizez,  1.0f,  1.0f,  1.0f, /* R */
		 bsizex,   sizey,   sizez,  1.0f,  1.0f,  1.0f, /* C */
		 sizex,   bsizey,   sizez,  1.0f,  1.0f,  1.0f, /* E */
		/* top front bevel */
		 sizex,    sizey,  bsizez,  0.0f,  1.0f,  1.0f, /* R */
		 sizex,   bsizey,   sizez,  0.0f,  1.0f,  1.0f, /* E */
		-sizex,    sizey,  bsizez,  0.0f,  1.0f,  1.0f, /* S */
		-sizex,    sizey,  bsizez,  0.0f,  1.0f,  1.0f, /* S */
		 sizex,   bsizey,   sizez,  0.0f,  1.0f,  1.0f, /* E */
		-sizex,   bsizey,   sizez,  0.0f,  1.0f,  1.0f, /* G */
		/* left top front corner */
		-sizex,    sizey,  bsizez, -1.0f,  1.0f,  1.0f, /* S */
		-sizex,   bsizey,   sizez, -1.0f,  1.0f,  1.0f, /* G */
		-bsizex,   sizey,   sizez, -1.0f,  1.0f,  1.0f, /* I */
		/* left front bevel */
		-sizex,    sizey,  bsizez, -1.0f,  0.0f,  1.0f, /* S */
		-bsizex,   sizey,   sizez, -1.0f,  0.0f,  1.0f, /* I */
		-sizex,   -sizey,  bsizez, -1.0f,  0.0f,  1.0f, /* T */
		-sizex,   -sizey,  bsizez, -1.0f,  0.0f,  1.0f, /* T */
		-bsizex,   sizey,   sizez, -1.0f,  0.0f,  1.0f, /* I */
		-bsizex,  -sizey,   sizez, -1.0f,  0.0f,  1.0f, /* K */
		/* left bottom front corner */
		-sizex,   -sizey,  bsizez, -1.0f, -1.0f,  1.0f, /* T */
		-bsizex,  -sizey,   sizez, -1.0f, -1.0f,  1.0f, /* K */
		-sizex,  -bsizey,   sizez, -1.0f, -1.0f,  1.0f, /* M */
		/* bottom front bevel */
		-sizex,  -bsizey,   sizez,  0.0f, -1.0f,  1.0f, /* M */
		 sizex,  -bsizey,   sizez,  0.0f, -1.0f,  1.0f, /* O */
		-sizex,   -sizey,  bsizez,  0.0f, -1.0f,  1.0f, /* T */
		-sizex,   -sizey,  bsizez,  0.0f, -1.0f,  1.0f, /* T */
		 sizex,  -bsizey,   sizez,  0.0f, -1.0f,  1.0f, /* O */
		 sizex,   -sizey,  bsizez,  0.0f, -1.0f,  1.0f, /* Q */
		/* right bottom front corner */
		 sizex,   -sizey,  bsizez,  1.0f, -1.0f,  1.0f, /* Q */
		 sizex,  -bsizey,   sizez,  1.0f, -1.0f,  1.0f, /* O */
		 bsizex,  -sizey,   sizez,  1.0f, -1.0f,  1.0f, /* A */
		/* front */
		-sizex,   -sizey,  bsizez,  0.0f,  0.0f,  1.0f, /* T */
		 sizex,   -sizey,  bsizez,  0.0f,  0.0f,  1.0f, /* Q */
		 sizex,    sizey,  bsizez,  0.0f,  0.0f,  1.0f, /* R */
		-sizex,   -sizey,  bsizez,  0.0f,  0.0f,  1.0f, /* T */
		 sizex,    sizey,  bsizez,  0.0f,  0.0f,  1.0f, /* R */
		-sizex,    sizey,  bsizez,  0.0f,  0.0f,  1.0f, /* S */
		/* right back bevel */
		bsizex,  -sizey,  -sizez,  1.0f,  0.0f, -1.0f, /* B */
		 sizex,  -sizey, -bsizez,  1.0f,  0.0f, -1.0f, /* V */
		bsizex,   sizey,  -sizez,  1.0f,  0.0f, -1.0f, /* D */
		bsizex,   sizey,  -sizez,  1.0f,  0.0f, -1.0f, /* D */
		 sizex,  -sizey, -bsizez,  1.0f,  0.0f, -1.0f, /* V */
		 sizex,   sizey, -bsizez,  1.0f,  0.0f, -1.0f, /* U */
		/* right top back corner */
		 sizex,   sizey, -bsizez,  1.0f,  1.0f, -1.0f, /* U */
		 sizex,  bsizey,  -sizez,  1.0f,  1.0f, -1.0f, /* F */
		bsizex,   sizey,  -sizez,  1.0f,  1.0f, -1.0f, /* D */
		/* top back bevel */
		 sizex,   sizey, -bsizez,  0.0f,  1.0f, -1.0f, /* U */
		-sizex,   sizey, -bsizez,  0.0f,  1.0f, -1.0f, /* X */
		 sizex,  bsizey,  -sizez,  0.0f,  1.0f, -1.0f, /* F */
		 sizex,  bsizey,  -sizez,  0.0f,  1.0f, -1.0f, /* F */
		-sizex,   sizey, -bsizez,  0.0f,  1.0f, -1.0f, /* X */
		-sizex,  bsizey,  -sizez,  0.0f,  1.0f, -1.0f, /* H */
		/* left top back corner */
		 -sizex,  bsizey,  -sizez, -1.0f,  1.0f, -1.0f, /* H */
		 -sizex,   sizey, -bsizez, -1.0f,  1.0f, -1.0f, /* X */
		-bsizex,   sizey,  -sizez, -1.0f,  1.0f, -1.0f, /* J */
		/* left back bevel */
		 -sizex,  -sizey, -bsizez, -1.0f,  0.0f, -1.0f, /* W */
		-bsizex,  -sizey,  -sizez, -1.0f,  0.0f, -1.0f, /* L */
		 -sizex,   sizey, -bsizez, -1.0f,  0.0f, -1.0f, /* X */
		 -sizex,   sizey, -bsizez, -1.0f,  0.0f, -1.0f, /* X */
		-bsizex,  -sizey,  -sizez, -1.0f,  0.0f, -1.0f, /* L */
		-bsizex,   sizey,  -sizez, -1.0f,  0.0f, -1.0f, /* J */
		/* left bottom back corner */
		 -sizex,  -sizey, -bsizez, -1.0f, -1.0f, -1.0f, /* W */
		 -sizex, -bsizey,  -sizez, -1.0f, -1.0f, -1.0f, /* N */
		-bsizex,  -sizey,  -sizez, -1.0f, -1.0f, -1.0f, /* L */
		/* bottom back bevel */
		  sizex, -bsizey,  -sizez,  0.0f, -1.0f, -1.0f, /* P */
		 -sizex, -bsizey,  -sizez,  0.0f, -1.0f, -1.0f, /* N */
		  sizex,  -sizey, -bsizez,  0.0f, -1.0f, -1.0f, /* V */
		  sizex,  -sizey, -bsizez,  0.0f, -1.0f, -1.0f, /* V */
		 -sizex, -bsizey,  -sizez,  0.0f, -1.0f, -1.0f, /* N */
		 -sizex,  -sizey, -bsizez,  0.0f, -1.0f, -1.0f, /* W */
		/* right bottom back corner */
		 bsizex,  -sizey,  -sizez,  1.0f, -1.0f, -1.0f, /* B */
		  sizex, -bsizey,  -sizez,  1.0f, -1.0f, -1.0f, /* P */
		  sizex,  -sizey, -bsizez,  1.0f, -1.0f, -1.0f, /* V */
		/* back */
		 sizex,   -sizey, -bsizez,  0.0f,  0.0f, -1.0f, /* V */
		-sizex,   -sizey, -bsizez,  0.0f,  0.0f, -1.0f, /* W */
		 sizex,    sizey, -bsizez,  0.0f,  0.0f, -1.0f, /* U */
		 sizex,    sizey, -bsizez,  0.0f,  0.0f, -1.0f, /* U */
		-sizex,   -sizey, -bsizez,  0.0f,  0.0f, -1.0f, /* W */
		-sizex,    sizey, -bsizez,  0.0f,  0.0f, -1.0f, /* X */
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

 	GLfloat *cube_verteces = 0;
	GLfloat *cube_normals = cube_verteces + 3;
	glEnable(GL_NORMALIZE);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glNormalPointer(GL_FLOAT, stride, cube_normals);
	glVertexPointer(3, GL_FLOAT, stride, cube_verteces);
	glScalef(0.6f, 0.6f, 0.6f);
	glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_NORMALIZE);
}

