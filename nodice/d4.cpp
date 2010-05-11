/**
 * @file nodice/d4.cpp
 * @brief Implemntation of the no-dice d4 module.
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
#include "nodice/d4.h"

#include <cmath>
#include "vmmlib/vmmlib.h"


namespace
{
	static const int row_width = 3*2;
} // anonymous namespace

NoDice::D4::
D4()
: Shape("d4", NoDice::Colour(1.0f, 1.0f, 0.0f, 0.40f))
{
	using vmml::Vector3f;

	const GLfloat size = 1.0f;
	const GLfloat half = 0.5773502692; // for tetrahedron in unit sphere

	const Vector3f A( half,  half,  half);
	const Vector3f B(-half,  half, -half);
	const Vector3f C(-half, -half,  half);
	const Vector3f D( half, -half, -half);

	const Vector3f normal1 = A.cross(B).getNormalized();
	const Vector3f normal2 = A.cross(C).getNormalized();
	const Vector3f normal3 = B.cross(D).getNormalized();
	const Vector3f normal4 = B.cross(A).getNormalized();

	/* vertex-3, normal-3 */
	GLfloat shape[] = {
	  A.x, A.y, A.z, normal1.x, normal1.y, normal1.z,
	  B.x, B.y, B.z, normal1.x, normal1.y, normal1.z,
	  C.x, C.y, C.z, normal1.x, normal1.y, normal1.z,

	  A.x, A.y, A.z, normal2.x, normal2.y, normal2.z,
	  C.x, C.y, C.z, normal2.x, normal2.y, normal2.z,
	  D.x, D.y, D.z, normal2.x, normal2.y, normal2.z,

	  B.x, B.y, B.z, normal3.x, normal3.y, normal3.z,
	  D.x, D.y, D.z, normal3.x, normal3.y, normal3.z,
	  C.x, C.y, C.z, normal3.x, normal3.y, normal3.z,

	  B.x, B.y, B.z, normal4.x, normal4.y, normal4.z,
	  A.x, A.y, A.z, normal4.x, normal4.y, normal4.z,
	  D.x, D.y, D.z, normal4.x, normal4.y, normal4.z,
	};
	m_vertexCount = (sizeof(shape) / sizeof(GLfloat)) / row_width;

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape), shape, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
} 


NoDice::D4::
~D4()
{
	glDeleteBuffers(1, &m_vbo);
} 


void NoDice::D4::
draw() const
{
 	static const int stride = row_width * sizeof(GLfloat);
 	static const GLfloat* shape_verteces = 0;
	static const GLfloat* shape_normals = shape_verteces + 3;

	glEnable(GL_NORMALIZE);
	glDisable(GL_CULL_FACE); // for transparency -- move to object
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glNormalPointer(GL_FLOAT, stride, shape_normals);
	glVertexPointer(3, GL_FLOAT, stride, shape_verteces);
	glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_NORMALIZE);
}

