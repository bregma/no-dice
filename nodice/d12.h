/**
 * @file nodice/d12.h
 * @brief Public interface of the no-dice d12 module.
 *
 * Copyright 2010, 2011 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
#ifndef NODICE_D12_H
#define NODICE_D12_H 1

#include "nodice/shape.h"
#include "nodice/video.h"


namespace NoDice
{

  /**
   * A 12-sided (dodecahedral) polyhedral die.
   */
  class D12
  : public Shape
  {
  public:
    D12();
    ~D12();
    int score();
    void draw() const;

  private:
    GLuint  m_vbo;
    GLsizei m_vertexCount;
  };
} // namespace noDice

#endif // NODICE_D12_H
