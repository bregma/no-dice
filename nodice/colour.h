/**
 * @file nodice/colour.h
 * @brief Public interface of the no-dice colour module.
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
#ifndef NODICE_COLOUR_H
#define NODICE_COLOUR_H 1

#include <vmmlib/vmmlib.h>


namespace NoDice
{
	typedef vmml::Vector4f Colour;

	extern Colour white;
	extern Colour red;

} // namespace noDice

#endif // NODICE_COLOUR_H
