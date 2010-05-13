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
#include "vmmlib/vmmlib.h"


namespace
{
	static const int row_width = 3*2;
} // anonymous namespace

NoDice::D12::
D12()
: Shape("d12", NoDice::Colour(0.3f, 0.5f, 1.0f, 0.80))
{
	using vmml::Vector3f;

	const GLfloat size = 1.0f;
	const GLfloat half = 0.5773502692; // for tetrahedron in unit sphere

	// Magic precomputed vertexes of the unit-sphere dodecahedron
	const Vector3f A( 0.607f,  0.000f,  0.795f);
	const Vector3f B( 0.188f,  0.577f,  0.795f);
	const Vector3f C(-0.491f,  0.357f,  0.795f);
	const Vector3f D(-0.491f, -0.357f,  0.795f);
	const Vector3f E( 0.188f, -0.577f,  0.795f);
	const Vector3f F( 0.982f,  0.000f,  0.188f);
	const Vector3f G( 0.304f,  0.934f,  0.188f);
	const Vector3f H(-0.795f,  0.577f,  0.188f);
	const Vector3f I(-0.795f, -0.577f,  0.188f);
	const Vector3f J( 0.304f, -0.934f,  0.188f);
	const Vector3f K( 0.795f,  0.577f, -0.188f);
	const Vector3f L(-0.304f,  0.934f, -0.188f);
	const Vector3f M(-0.982f,  0.000f, -0.188f);
	const Vector3f N(-0.304f, -0.934f, -0.188f);
	const Vector3f O( 0.795f, -0.577f, -0.188f);
	const Vector3f P( 0.491f,  0.375f, -0.795f);
	const Vector3f Q(-0.188f,  0.577f, -0.795f);
	const Vector3f R(-0.607f,  0.000f, -0.795f);
	const Vector3f S(-0.188f, -0.577f, -0.795f);
	const Vector3f T( 0.491f, -0.357f, -0.795f);

	const Vector3f normal1  = (D - A).cross(C - D).getNormalized();
	const Vector3f normal2  = (K - A).cross(G - K).getNormalized();
	const Vector3f normal3  = (L - B).cross(H - L).getNormalized();
	const Vector3f normal4  = (M - C).cross(I - M).getNormalized();
	const Vector3f normal5  = (N - D).cross(J - N).getNormalized();
	const Vector3f normal6  = (F - E).cross(F - O).getNormalized();
	const Vector3f normal7  = (G - P).cross(L - G).getNormalized();
	const Vector3f normal8  = (D - Q).cross(M - D).getNormalized();
	const Vector3f normal9  = (I - R).cross(N - I).getNormalized();
	const Vector3f normal10 = (J - S).cross(O - J).getNormalized();
	const Vector3f normal11 = (F - T).cross(K - F).getNormalized();
	const Vector3f normal12 = (S - P).cross(R - S).getNormalized();

	/* vertex-3, normal-3 */
	GLfloat shape[] = {
	  // face 1
	  A.x, A.y, A.z, normal1.x, normal1.y, normal1.z,
	  C.x, C.y, C.z, normal1.x, normal1.y, normal1.z,
	  B.x, B.y, B.z, normal1.x, normal1.y, normal1.z,

	  A.x, A.y, A.z, normal1.x, normal1.y, normal1.z,
	  D.x, D.y, D.z, normal1.x, normal1.y, normal1.z,
	  C.x, C.y, C.z, normal1.x, normal1.y, normal1.z,

	  A.x, A.y, A.z, normal1.x, normal1.y, normal1.z,
	  E.x, E.y, E.z, normal1.x, normal1.y, normal1.z,
	  D.x, D.y, D.z, normal1.x, normal1.y, normal1.z,

	  // face 2
	  A.x, A.y, A.z, normal2.x, normal2.y, normal2.z,
	  G.x, G.y, G.z, normal2.x, normal2.y, normal2.z,
	  B.x, B.y, B.z, normal2.x, normal2.y, normal2.z,

	  A.x, A.y, A.z, normal2.x, normal2.y, normal2.z,
	  K.x, K.y, K.z, normal2.x, normal2.y, normal2.z,
	  G.x, G.y, G.z, normal2.x, normal2.y, normal2.z,

	  A.x, A.y, A.z, normal2.x, normal2.y, normal2.z,
	  F.x, F.y, F.z, normal2.x, normal2.y, normal2.z,
	  K.x, K.y, K.z, normal2.x, normal2.y, normal2.z,

	  // face 3
	  B.x, B.y, B.z, normal3.x, normal3.y, normal3.z,
	  H.x, H.y, H.z, normal3.x, normal3.y, normal3.z,
	  C.x, C.y, C.z, normal3.x, normal3.y, normal3.z,

	  B.x, B.y, B.z, normal3.x, normal3.y, normal3.z,
	  L.x, L.y, L.z, normal3.x, normal3.y, normal3.z,
	  H.x, H.y, H.z, normal3.x, normal3.y, normal3.z,

	  B.x, B.y, B.z, normal3.x, normal3.y, normal3.z,
	  G.x, G.y, G.z, normal3.x, normal3.y, normal3.z,
	  L.x, L.y, L.z, normal3.x, normal3.y, normal3.z,

	  // face 4
	  C.x, C.y, C.z, normal4.x, normal4.y, normal4.z,
	  I.x, I.y, I.z, normal4.x, normal4.y, normal4.z,
	  D.x, D.y, D.z, normal4.x, normal4.y, normal4.z,

	  C.x, C.y, C.z, normal4.x, normal4.y, normal4.z,
	  M.x, M.y, M.z, normal4.x, normal4.y, normal4.z,
	  I.x, I.y, I.z, normal4.x, normal4.y, normal4.z,

	  C.x, C.y, C.z, normal4.x, normal4.y, normal4.z,
	  H.x, H.y, H.z, normal4.x, normal4.y, normal4.z,
	  M.x, M.y, M.z, normal4.x, normal4.y, normal4.z,

	  // face 5
	  D.x, D.y, D.z, normal5.x, normal5.y, normal5.z,
	  J.x, J.y, J.z, normal5.x, normal5.y, normal5.z,
	  E.x, E.y, E.z, normal5.x, normal5.y, normal5.z,

	  D.x, D.y, D.z, normal5.x, normal5.y, normal5.z,
	  N.x, N.y, N.z, normal5.x, normal5.y, normal5.z,
	  J.x, J.y, J.z, normal5.x, normal5.y, normal5.z,

	  D.x, D.y, D.z, normal5.x, normal5.y, normal5.z,
	  I.x, I.y, I.z, normal5.x, normal5.y, normal5.z,
	  N.x, N.y, N.z, normal5.x, normal5.y, normal5.z,

	  // face 6
	  E.x, E.y, E.z, normal6.x, normal6.y, normal6.z,
	  F.x, F.y, F.z, normal6.x, normal6.y, normal6.z,
	  A.x, A.y, A.z, normal6.x, normal6.y, normal6.z,

	  E.x, E.y, E.z, normal6.x, normal6.y, normal6.z,
	  O.x, O.y, O.z, normal6.x, normal6.y, normal6.z,
	  F.x, F.y, F.z, normal6.x, normal6.y, normal6.z,

	  E.x, E.y, E.z, normal6.x, normal6.y, normal6.z,
	  J.x, J.y, J.z, normal6.x, normal6.y, normal6.z,
	  O.x, O.y, O.z, normal6.x, normal6.y, normal6.z,

	  // face 7
	  P.x, P.y, P.z, normal7.x, normal7.y, normal7.z,
	  L.x, L.y, L.z, normal7.x, normal7.y, normal7.z,
	  Q.x, Q.y, Q.z, normal7.x, normal7.y, normal7.z,

	  P.x, P.y, P.z, normal7.x, normal7.y, normal7.z,
	  G.x, G.y, G.z, normal7.x, normal7.y, normal7.z,
	  L.x, L.y, L.z, normal7.x, normal7.y, normal7.z,

	  P.x, P.y, P.z, normal7.x, normal7.y, normal7.z,
	  K.x, K.y, K.z, normal7.x, normal7.y, normal7.z,
	  G.x, G.y, G.z, normal7.x, normal7.y, normal7.z,

	  // face 8
	  Q.x, Q.y, Q.z, normal8.x, normal8.y, normal8.z,
	  M.x, M.y, M.z, normal8.x, normal8.y, normal8.z,
	  R.x, R.y, R.z, normal8.x, normal8.y, normal8.z,

	  Q.x, Q.y, Q.z, normal8.x, normal8.y, normal8.z,
	  H.x, H.y, H.z, normal8.x, normal8.y, normal8.z,
	  M.x, M.y, M.z, normal8.x, normal8.y, normal8.z,

	  Q.x, Q.y, Q.z, normal8.x, normal8.y, normal8.z,
	  L.x, L.y, L.z, normal8.x, normal8.y, normal8.z,
	  H.x, H.y, H.z, normal8.x, normal8.y, normal8.z,

	  // face 9
	  R.x, R.y, R.z, normal9.x, normal9.y, normal9.z,
	  N.x, N.y, N.z, normal9.x, normal9.y, normal9.z,
	  S.x, S.y, S.z, normal9.x, normal9.y, normal9.z,

	  R.x, R.y, R.z, normal9.x, normal9.y, normal9.z,
	  I.x, I.y, I.z, normal9.x, normal9.y, normal9.z,
	  N.x, N.y, N.z, normal9.x, normal9.y, normal9.z,

	  R.x, R.y, R.z, normal9.x, normal9.y, normal9.z,
	  M.x, M.y, M.z, normal9.x, normal9.y, normal9.z,
	  I.x, I.y, I.z, normal9.x, normal9.y, normal9.z,

	  // face 10
	  S.x, S.y, S.z, normal10.x, normal10.y, normal10.z,
	  O.x, O.y, O.z, normal10.x, normal10.y, normal10.z,
	  T.x, T.y, T.z, normal10.x, normal10.y, normal10.z,

	  S.x, S.y, S.z, normal10.x, normal10.y, normal10.z,
	  J.x, J.y, J.z, normal10.x, normal10.y, normal10.z,
	  O.x, O.y, O.z, normal10.x, normal10.y, normal10.z,

	  S.x, S.y, S.z, normal10.x, normal10.y, normal10.z,
	  N.x, N.y, N.z, normal10.x, normal10.y, normal10.z,
	  J.x, J.y, J.z, normal10.x, normal10.y, normal10.z,

	  // face 11
	  T.x, T.y, T.z, normal11.x, normal11.y, normal11.z,
	  K.x, K.y, K.z, normal11.x, normal11.y, normal11.z,
	  P.x, P.y, P.z, normal11.x, normal11.y, normal11.z,

	  T.x, T.y, T.z, normal11.x, normal11.y, normal11.z,
	  F.x, F.y, F.z, normal11.x, normal11.y, normal11.z,
	  K.x, K.y, K.z, normal11.x, normal11.y, normal11.z,

	  T.x, T.y, T.z, normal11.x, normal11.y, normal11.z,
	  O.x, O.y, O.z, normal11.x, normal11.y, normal11.z,
	  F.x, F.y, F.z, normal11.x, normal11.y, normal11.z,

	  // face 12
	  P.x, P.y, P.z, normal12.x, normal12.y, normal12.z,
	  R.x, R.y, R.z, normal12.x, normal12.y, normal12.z,
	  Q.x, Q.y, Q.z, normal12.x, normal12.y, normal12.z,

	  P.x, P.y, P.z, normal12.x, normal12.y, normal12.z,
	  S.x, S.y, S.z, normal12.x, normal12.y, normal12.z,
	  R.x, R.y, R.z, normal12.x, normal12.y, normal12.z,

	  P.x, P.y, P.z, normal12.x, normal12.y, normal12.z,
	  T.x, T.y, T.z, normal12.x, normal12.y, normal12.z,
	  S.x, S.y, S.z, normal12.x, normal12.y, normal12.z,
	};
	m_vertexCount = (sizeof(shape) / sizeof(GLfloat)) / row_width;

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape), shape, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

