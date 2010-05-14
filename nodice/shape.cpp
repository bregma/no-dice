/**
 * @file nodice/shape.cpp
 * @brief Implemntation of the nodice/shape module.
 *
 * Copyright 2009, 2010 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
#include "nodice/shape.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include "nodice/d4.h"
#include "nodice/d6.h"
#include "nodice/d8.h"
#include "nodice/d12.h"
#include <vector>

namespace
{
	typedef std::vector<NoDice::ShapePtr> ShapeBag;

	ShapeBag
	generateShapeBag()
	{
		ShapeBag bag;

		bag.push_back(NoDice::ShapePtr(new NoDice::D4));
		bag.push_back(NoDice::ShapePtr(new NoDice::D6)); 
		bag.push_back(NoDice::ShapePtr(new NoDice::D8)); 
		bag.push_back(NoDice::ShapePtr(new NoDice::D12)); 

		return bag;
	}
} // anonymous namespace


NoDice::Shape::
Shape(const std::string& name,
			const Colour&      defaultColour)
: m_name(name)
, m_defaultColour(defaultColour)
{
}


NoDice::Shape::
~Shape()
{
}


const std::string& NoDice::Shape::
name() const
{
  return m_name;
}


const NoDice::Colour& NoDice::Shape::
defaultColour() const
{
  return m_defaultColour;
}


NoDice::ShapePtr NoDice::
chooseAShape()
{
	static ShapeBag s_shapeBag = generateShapeBag();

	int r = (rand() >> 2) % s_shapeBag.size();
	return s_shapeBag.at(r);
}


/**
 * Generates the VBO entries for a pentagon.
 * @param[in]  vertex Array of vertexes 
 * @param[in]  index  Index of 5 vertexes in CCW order
 * @param[out] buffer Output buffer
 *
 * The index must provide vertexes in the correct CCW winding order.
 */
void NoDice::
pentagon(const vmml::Vector3f vertex[], const int index[5], GLfloat*& buffer)
{
	vmml::Vector3f v1 = (vertex[index[1]] - vertex[index[0]]);
	vmml::Vector3f v2 = (vertex[index[2]] - vertex[index[0]]);
	vmml::Vector3f normal = v1.cross(v2);

	buffer = std::copy(vertex[index[0]].begin(), vertex[index[0]].end(), buffer);
	buffer = std::copy(normal.begin(), normal.end(), buffer);
	buffer = std::copy(vertex[index[1]].begin(), vertex[index[1]].end(), buffer);
	buffer = std::copy(normal.begin(), normal.end(), buffer);
	buffer = std::copy(vertex[index[2]].begin(), vertex[index[2]].end(), buffer);
	buffer = std::copy(normal.begin(), normal.end(), buffer);
	buffer = std::copy(vertex[index[0]].begin(), vertex[index[0]].end(), buffer);
	buffer = std::copy(normal.begin(), normal.end(), buffer);
	buffer = std::copy(vertex[index[2]].begin(), vertex[index[2]].end(), buffer);
	buffer = std::copy(normal.begin(), normal.end(), buffer);
	buffer = std::copy(vertex[index[3]].begin(), vertex[index[3]].end(), buffer);
	buffer = std::copy(normal.begin(), normal.end(), buffer);
	buffer = std::copy(vertex[index[0]].begin(), vertex[index[0]].end(), buffer);
	buffer = std::copy(normal.begin(), normal.end(), buffer);
	buffer = std::copy(vertex[index[3]].begin(), vertex[index[3]].end(), buffer);
	buffer = std::copy(normal.begin(), normal.end(), buffer);
	buffer = std::copy(vertex[index[4]].begin(), vertex[index[4]].end(), buffer);
	buffer = std::copy(normal.begin(), normal.end(), buffer);
}

