/**
 * @file nodice/maths.h
 * @brief Public interface of the no-dice maths module.
 *
 * This file is called "maths.h" so it does not conflict with the C standard
 * header <math.h> and confound build machinery that provides broken search
 * paths for headers.
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
#ifndef NODICE_MATH_H
#define NODICE_MATH_H 1

#include <vmmlib/vmmlib.h>


namespace NoDice
{
	using vmml::Vector2f;
	using vmml::Vector2i;
	using vmml::Vector3f;
	using vmml::Vector4f;
	using vmml::Matrix4f;

	/** Number of Cartesian coordinates in a 3D vertex. */
	const int coords_per_vertex = 3;

	/** Number of Cartesian coordinates in a 3D normal. */
	const int coords_per_normal = 3;

	/** Number of vertexes in a triangle. */
	const int vertexes_per_triangle = 3;

} // namespace NoDice

#endif // NODICE_MATH_H
