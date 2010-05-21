/**
 * @file nodice/d12.cpp
 * @brief Implemntation of the no-dice d12 module.
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
#include "nodice/d12.h"

#include <cmath>
#include <iostream>
#include "nodice/maths.h"


namespace
{
	static const int triangles_per_face = 3;
	static const int row_width          = NoDice::coords_per_vertex
	                                    + NoDice::coords_per_normal;
} // anonymous namespace

NoDice::D12::
D12()
: Shape("d12", NoDice::Colour(0.3f, 0.5f, 1.0f, 0.80))
{
	// Magic precomputed vertexes of the unit-sphere dodecahedron
	static const Vector3f vertex[] = 
	{
		Vector3f( 0.607f,  0.000f,  0.795f),
		Vector3f( 0.188f,  0.577f,  0.795f),
		Vector3f(-0.491f,  0.357f,  0.795f),
		Vector3f(-0.491f, -0.357f,  0.795f),
		Vector3f( 0.188f, -0.577f,  0.795f),
		Vector3f( 0.982f,  0.000f,  0.188f),
		Vector3f( 0.304f,  0.934f,  0.188f),
		Vector3f(-0.795f,  0.577f,  0.188f),
		Vector3f(-0.795f, -0.577f,  0.188f),
		Vector3f( 0.304f, -0.934f,  0.188f),
		Vector3f( 0.795f,  0.577f, -0.188f),
		Vector3f(-0.304f,  0.934f, -0.188f),
		Vector3f(-0.982f,  0.000f, -0.188f),
		Vector3f(-0.304f, -0.934f, -0.188f),
		Vector3f( 0.795f, -0.577f, -0.188f),
		Vector3f( 0.491f,  0.375f, -0.795f),
		Vector3f(-0.188f,  0.577f, -0.795f),
		Vector3f(-0.607f,  0.000f, -0.795f),
		Vector3f(-0.188f, -0.577f, -0.795f),
		Vector3f( 0.491f, -0.357f, -0.795f)
	};

	// List of vertexes making up a face.
	typedef int FaceVertexIndex[5];

	// Table of vertexes making up the faces of the dodecahedron.
	static const FaceVertexIndex index[] = 
	{
		{  0,  1,  2,  3,  4 },
		{  5, 10,  6,  1,  0 },
		{  6, 11,  7,  2,  1 },
		{  7, 12,  8,  3,  2 },
		{  8, 13,  9,  4,  3 },
		{  9, 14,  5,  0,  4 },
		{ 15, 16, 11,  6, 10 },
		{ 16, 17, 12,  7, 11 },
		{ 17, 18, 13,  8, 12 },
		{ 18, 19, 14,  9, 13 },
		{ 19, 15, 10,  5, 14 },
		{ 19, 18, 17, 16, 15 }
	};

	static const int num_faces = sizeof(index) / sizeof(FaceVertexIndex);
	static const int vertex_count = num_faces * triangles_per_face
	                                          * vertexes_per_triangle;

	// A temporary buffer with which the VBO will be initialized
	GLfloat shape[row_width * vertex_count];
	GLfloat* p = shape;
	for (int i = 0; i < num_faces; ++i)
	{
		pentagon(vertex, index[i], p);
	}

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape), shape, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_vertexCount = vertex_count;
} 


NoDice::D12::
~D12()
{
	glDeleteBuffers(1, &m_vbo);
} 


void NoDice::D12::
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

