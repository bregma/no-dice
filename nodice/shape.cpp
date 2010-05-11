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

#include <cstdlib>
#include "nodice/d4.h"
#include "nodice/d6.h"
#include "nodice/d8.h"
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
	std::cerr << "==smw> " << __PRETTY_FUNCTION__ << " r=" << r << "\n";
	return s_shapeBag.at(r);
}


