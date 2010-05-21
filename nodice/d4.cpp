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
#include "nodice/maths.h"


namespace
{
	static const int triangles_per_face = 1;
	static const int row_width          = NoDice::coords_per_vertex
	                                    + NoDice::coords_per_normal;
} // anonymous namespace

NoDice::D4::
D4()
: Shape("d4", NoDice::Colour(1.0f, 1.0f, 0.0f, 0.60f))
{
	static const GLfloat half = 0.5773502692; // for tetrahedron in unit sphere

	static const Vector3f vertex[] =
	{
		Vector3f( half,  half,  half),
		Vector3f(-half,  half, -half),
		Vector3f(-half, -half,  half),
		Vector3f( half, -half, -half)
	};

	// List of vertexes making up a face.
	typedef int FaceVertexIndex[3];

	// Table of vertexes making up the faces of the tetrahedron.
	static const FaceVertexIndex index[] =
	{
		{ 1, 2, 0 },
		{ 2, 3, 0 },
		{ 3, 2, 1 },
		{ 0, 3, 1 }
	};

	static const int num_faces = sizeof(index) / sizeof(FaceVertexIndex);
	static const int vertex_count = num_faces * triangles_per_face
	                                          * vertexes_per_triangle;

	// A temporary buffer with which the VBO will be initialized
	GLfloat shape[row_width * vertex_count];
	GLfloat* p = shape;
	for (int i = 0; i < num_faces; ++i)
	{
		triangle(vertex, index[i], p);
	}

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape), shape, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Remember the size of the VBO.
	m_vertexCount = vertex_count;
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
	static const GLfloat* shape_normals = shape_verteces + coords_per_vertex;

	glEnable(GL_NORMALIZE);
	glDisable(GL_CULL_FACE); // for transparency -- move to object
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glNormalPointer(GL_FLOAT, stride, shape_normals);
	glVertexPointer(coords_per_vertex, GL_FLOAT, stride, shape_verteces);
	glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_NORMALIZE);
}

