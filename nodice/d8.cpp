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
#include "nodice/maths.h"


namespace
{
	static const int triangles_per_face = 1;
	static const int row_width          = NoDice::coords_per_vertex
	                                    + NoDice::coords_per_normal;
} // anonymous namespace


/**
 * Generates an octohedron in the unit spehere.
 */
NoDice::D8::
D8()
: Shape("d8", NoDice::Colour(0.8f, 0.0f, 0.8f, 0.48f))
{
	// The octahedron is inscribed inside a sphere with this radius.
	static const GLfloat r = 1.0f;
	static const GLfloat z = 0.0f;

	static const Vector3f vertex[] = 
	{
		Vector3f( r,  z,  z),
		Vector3f( z,  r,  z),
		Vector3f( z,  z,  r),
		Vector3f(-r,  z,  z),
		Vector3f( z,  z, -r),
		Vector3f( z, -r,  z)
	};

	// List of vertexes making up a face.
	typedef int FaceVertexIndex[3];

	// Table of vertexes making up the faces of the octohedron
	static const FaceVertexIndex index[] =
	{
		 { 0, 1, 2 },
		 { 2, 1, 3 },
		 { 3, 1, 4 },
		 { 4, 1, 0 },
		 { 0, 5, 4 },
		 { 4, 5, 3 },
		 { 3, 5, 2 },
		 { 2, 5, 0 }
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

	// Create the VBO.
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape), shape, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Remember the size of the VBO.
	m_vertexCount = vertex_count;
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
	static const GLfloat* shape_normals = shape_verteces + coords_per_vertex;

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

