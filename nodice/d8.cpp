/**
 * @file nodice/d8.cpp
 * @brief Implemntation of the no-dice d8 module.
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
#include "nodice/d8.h"

#include <cmath>
#include "vmmlib/vmmlib.h"


namespace
{
	static const int row_width = 3*2;
} // anonymous namespace

/**
 * Generates an octohedron in the unit spehere.
 *
 * Uh, this is wrong.  This gives a 6-sided polyhedron, we need an eight-sided
 * polyhedron.  Rethink this.
 */
NoDice::D8::
D8()
: Shape("d8", NoDice::Colour(0.8f, 0.0f, 0.8f, 0.48f))
{
	using vmml::Vector3f;

	// the octahedron is inscribed inside a sphere with this radius
	static const GLfloat r = 1.0f;
	static const GLfloat z = 0.0f;

	Vector3f vertex[6];
	vertex[0].set( r,  z,  z);
	vertex[1].set( z,  r,  z);
	vertex[2].set( z,  z,  r);
	vertex[3].set(-r,  z,  z);
	vertex[4].set( z,  z, -r);
	vertex[5].set( z, -r,  z);

	Vector3f normal[8];
	normal[0] = (vertex[0] - vertex[1]).cross(vertex[2] - vertex[1]).getNormalized();
	normal[1] = (vertex[2] - vertex[1]).cross(vertex[3] - vertex[1]).getNormalized();
	normal[2] = (vertex[3] - vertex[1]).cross(vertex[4] - vertex[1]).getNormalized();
	normal[3] = (vertex[4] - vertex[1]).cross(vertex[0] - vertex[1]).getNormalized();
	normal[4] = (vertex[0] - vertex[5]).cross(vertex[4] - vertex[5]).getNormalized();
	normal[5] = (vertex[4] - vertex[5]).cross(vertex[3] - vertex[5]).getNormalized();
	normal[6] = (vertex[3] - vertex[5]).cross(vertex[2] - vertex[5]).getNormalized();
	normal[7] = (vertex[2] - vertex[5]).cross(vertex[0] - vertex[5]).getNormalized();
	
	/* vertex-3, normal-3 */
	GLfloat shape[] = {
		vertex[0]. x, vertex[0].y, vertex[0].z, normal[0].x, normal[0].y, normal[0].z,
		vertex[1]. x, vertex[1].y, vertex[1].z, normal[0].x, normal[0].y, normal[0].z,
		vertex[2]. x, vertex[2].y, vertex[2].z, normal[0].x, normal[0].y, normal[0].z,

		vertex[2]. x, vertex[2].y, vertex[2].z, normal[1].x, normal[1].y, normal[1].z,
		vertex[1]. x, vertex[1].y, vertex[1].z, normal[1].x, normal[1].y, normal[1].z,
		vertex[3]. x, vertex[3].y, vertex[3].z, normal[1].x, normal[1].y, normal[1].z,

		vertex[3]. x, vertex[3].y, vertex[3].z, normal[2].x, normal[2].y, normal[2].z,
		vertex[1]. x, vertex[1].y, vertex[1].z, normal[2].x, normal[2].y, normal[2].z,
		vertex[4]. x, vertex[4].y, vertex[4].z, normal[2].x, normal[2].y, normal[2].z,

		vertex[4]. x, vertex[4].y, vertex[4].z, normal[3].x, normal[3].y, normal[3].z,
		vertex[1]. x, vertex[1].y, vertex[1].z, normal[1].x, normal[3].y, normal[3].z,
		vertex[0]. x, vertex[0].y, vertex[0].z, normal[3].x, normal[3].y, normal[3].z,

		vertex[0]. x, vertex[0].y, vertex[0].z, normal[4].x, normal[4].y, normal[4].z,
		vertex[5]. x, vertex[5].y, vertex[5].z, normal[4].x, normal[4].y, normal[4].z,
		vertex[4]. x, vertex[4].y, vertex[4].z, normal[4].x, normal[4].y, normal[4].z,

		vertex[4]. x, vertex[4].y, vertex[4].z, normal[5].x, normal[5].y, normal[5].z,
		vertex[5]. x, vertex[5].y, vertex[5].z, normal[5].x, normal[5].y, normal[5].z,
		vertex[3]. x, vertex[3].y, vertex[3].z, normal[5].x, normal[5].y, normal[5].z,

		vertex[3]. x, vertex[3].y, vertex[3].z, normal[6].x, normal[6].y, normal[6].z,
		vertex[5]. x, vertex[5].y, vertex[5].z, normal[6].x, normal[6].y, normal[6].z,
		vertex[2]. x, vertex[2].y, vertex[2].z, normal[6].x, normal[6].y, normal[6].z,

		vertex[2]. x, vertex[2].y, vertex[2].z, normal[7].x, normal[7].y, normal[7].z,
		vertex[5]. x, vertex[5].y, vertex[5].z, normal[7].x, normal[7].y, normal[7].z,
		vertex[0]. x, vertex[0].y, vertex[0].z, normal[7].x, normal[7].y, normal[7].z,

	};
	m_vertexCount = (sizeof(shape) / sizeof(GLfloat)) / row_width;

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape), shape, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
} 


NoDice::D8::
~D8()
{
	glDeleteBuffers(1, &m_vbo);
} 


void NoDice::D8::
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

