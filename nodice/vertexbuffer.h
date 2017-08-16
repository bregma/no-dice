/**
 * @file vertexbuffer.h
 * @brief Public interface of the vertex buffer module.
 */
/*
 * Copyright 2017 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
#ifndef NODICE_VERTEXBUFFER_H
#define NODICE_VERTEXBUFFER_H 1

#include "nodice/opengl.h"


namespace NoDice
{
  /**
   * RAII wrapper around an OpenGL Vertex Buffer Object.
   */
  class VertexBuffer
  {
  public:
    /**
     * Creates an RAII object for a vertex buffer (VBO) bound to @p target.
     * @param[in] target OpenGL buffer
     * @param[in] size of the buffrer in bytes
     * @param[in] data pointer, nullptr indicates don't initialize
     * @param[in] usage hint
     *
     * @p target can include GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER,
     * and a host of other things that a VertexBuffer can hold depending on the 
     * version and variant of OpenGL.
     */
    VertexBuffer(GLenum target, GLsizeiptr size, GLvoid const* data, GLenum usage);

    ~VertexBuffer();

    void
    bind() const;

    /**
     * Copy data into all or part of the buffer.
     * @param[in] offset from start of buffer in bytes
     * @param[in] size of the data in bytes
     * @param[in] data pointer
     */
    void
    copy_in(GLintptr offset, GLsizeiptr size, GLvoid const* data);

    void
    unbind() const;

  private:
    VertexBuffer(VertexBuffer const&) = delete;
    VertexBuffer& operator=(VertexBuffer const&) = delete;

  public:
    GLuint vbo_;
    GLenum target_;
  };


  /**
   * RAII object for scoped binding of a VertexBuffer.
   */
  class VertexBufferBinding
  {
  public:
    VertexBufferBinding(VertexBuffer const& vbo)
    : vbo_(vbo)
    {
      vbo_.bind();
    }

    ~VertexBufferBinding()
    {
      vbo_.unbind();
    }

  private:
    VertexBuffer const& vbo_;
  };
} // namespace NoDice

#endif // NODICE_VertexBuffer_H
