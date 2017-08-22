/**
 * @file openglmesh.h
 * @brief Public interface of the No-Dice OpenGL Mesh module.
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
#ifndef NODICE_OPENGLMESH_H
#define NODICE_OPENGLMESH_H 1

#include "nodice/mesh.h"
#include "nodice/opengl.h"


namespace NoDice
{
  /**
   * A Mesh that uses OpenGL calls to store its data in GPU memory.
   */
  class OpenGLMesh
  : public Mesh
  {
  public:
    OpenGLMesh();

    ~OpenGLMesh();

  private:
    void
    store_vertex_data(float const* data) override;

    void
    reset_vertex_data() override;

    void
    store_index_data(std::uint16_t const* data) override;

    void
    reset_index_data() override;

    void
    draw_indexed() override;

    void
    draw_direct() override;

  private:
    GLuint vao;
    GLuint vbo[2];
  };

} // namespace NoDice

#endif // NODICE_OPENGLMESH_H
