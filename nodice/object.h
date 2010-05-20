/**
 * @file nodice/object.h
 * @brief Public interface of the nodice/object module.
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
#ifndef NODICE_OBJECT_H
#define NODICE_OBJECT_H 1

#include "nodice/colour.h"
#include "nodice/shape.h"
#include <tr1/memory>


namespace NoDice
{
  class Shape;

  /**
   * An instance of a drawable object.
   *
   * A drawable object instance has a shape, a transform (modelview matrix),
   * a colour, and um, I dunno, other state.
   */
  class Object
  {
  public:
    /** Constructs the object with a given shape. */
    Object(const ShapePtr shape);

    /** Destroys the object. */
    virtual ~Object();

    /** Gets the name of the type of the object. */
    virtual const std::string& type() const;

    /** Turns on highlight mode. */
		void setHighlight(bool toggle);

    /** Performs a one-tick update of the object. */
    virtual void update();

		/** Renders the object on the current drawing surface. */
    virtual void draw() const;

    /**
     * @name Disappearance
     * Member functions used to animate fading of an object.
     */
    /*@{*/

    /** Indicates if the object has completely disappeared. */
    bool hasDisappeared() const;

    /** Starts the object disappearing. */
    void startDisappearing();
    /*@}*/

  private:
    Object(const Object&);
    Object& operator=(const Object&);

  protected:
    const ShapePtr m_shape;
    Colour         m_colour;
    Colour         m_normalColour;
    Colour         m_highlightColour;
    bool           m_isDisappearing;
    float          m_fadeFactor;
    int            m_xrot, m_yrot; // temp for testing
  };

	/** Points to an object. */
	typedef std::tr1::shared_ptr<Object> ObjectPtr;
} // namespace NoDice

#endif // NODICE_OBJECT_H
