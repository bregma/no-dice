/**
 * @file nodice/colour.h
 * @brief Public interface of the no-dice colour module.
 */
/*
 * Copyright 2010, 2017 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
 *
 * This file is part of no-dice.
 *
 * No-dice is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * No-dice is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with no-dice.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef NODICE_COLOUR_H
#define NODICE_COLOUR_H 1

#include <nodice/maths.h>


namespace NoDice
{
  using Colour = vec4;

  extern Colour white;
  extern Colour red;

} // namespace noDice

#endif // NODICE_COLOUR_H
