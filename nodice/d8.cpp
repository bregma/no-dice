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
 * Generates a bipyramidal polyhedron in the unit spehere.
 *
 * Uh, this is wrong.  This gives a 6-sided polyhedron, we need an eight-sided
 * polyhedron.  Rethink this.
 */
NoDice::D8::
D8()
: Shape("d8", NoDice::Colour(0.8f, 0.0f, 0.8f, 0.48f))
{
	using vmml::Vector3f;

	static const GLfloat size = 1.0f;
	static const GLfloat pi = 3.14159265358979323846f;
	static const GLfloat cos60 = std::cos(pi * 30.0f / 180.0f);
	static const GLfloat sin60 = std::sin(pi * 30.0f / 180.0f);

	const Vector3f top(0.0f, 1.0f, 0.0f);
	const Vector3f bottom(0.0f, -1.0f, 0.0f);
	const Vector3f back(0.0f, 0.0f, -1.0f);
	const Vector3f left(-cos60, 0.0f, sin60);
	const Vector3f right(cos60, 0.0f, sin60);

	const Vector3f normal1 = right.cross(top).getNormalized();
	const Vector3f normal2 = left.cross(bottom).getNormalized();
	const Vector3f normal3 = right.cross(bottom).getNormalized();
	const Vector3f normal4 = back.cross(top).getNormalized();
	const Vector3f normal5 = top.cross(back).getNormalized();
	const Vector3f normal6 = left.cross(bottom).getNormalized();

	/* vertex-3, normal-3 */
	GLfloat shape[] = {
		right.x,  right.y,  right.z,  normal1.x, normal1.y, normal1.z,
		top.x,    top.y,    top.z,    normal1.x, normal1.y, normal1.z,
		left.x,   left.y,   left.z,   normal1.x, normal1.y, normal1.z,

		left.x,   left.y,   left.z,   normal2.x, normal2.y, normal2.z,
		bottom.x, bottom.y, bottom.z, normal2.x, normal2.y, normal2.z,
		right.x,  right.y,  right.z,  normal2.x, normal2.y, normal2.z,

		right.x,  right.y,  right.z,  normal3.x, normal3.y, normal3.z,
		bottom.x, bottom.y, bottom.z, normal3.x, normal3.y, normal3.z,
		back.x,   back.y,   back.z,   normal3.x, normal3.y, normal3.z,

		back.x,   back.y,   back.z,   normal4.x, normal4.y, normal4.z,
		top.x,    top.y,    top.z,    normal4.x, normal4.y, normal4.z,
		right.x,  right.y,  right.z,  normal4.x, normal4.y, normal4.z,

		top.x,    top.y,    top.z,    normal5.x, normal5.y, normal5.z,
		back.x,   back.y,   back.z,   normal5.x, normal5.y, normal5.z,
		left.x,   left.y,   left.z,   normal5.x, normal5.y, normal5.z,

		left.x,   left.y,   left.z,   normal6.x, normal6.y, normal6.z,
		bottom.x, bottom.y, bottom.z, normal6.x, normal6.y, normal6.z,
		back.x,   back.y,   back.z,   normal6.x, normal6.y, normal6.z,
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

