/**
 * @file tests/test_renderable.cpp
 * @brief Implemntation of the Renderable submodule.
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
#include "catch/catch.hpp"
#include "nodice/renderable.h"

#include "nodice/mesh.h"
#include <stdexcept>


namespace
{
  class MockMesh
  : public NoDice::Mesh
  {
  public:
    MockMesh()
    : constructionCount(1) 
    { }

    ~MockMesh()
    { --this->constructionCount; }

    int
    count() const
    { return this->constructionCount; }

    void store_vertex_data(float const*) override { }
    void reset_vertex_data() override { }
    void store_index_data(std::uint16_t const*) override { }
    void reset_index_data() override { }
    void activate() override { }
    bool is_active_p() const override { return true; }
    void deactivate() override { }
    void draw_direct() override {  }
    void draw_indexed() override { }

  private:
    int constructionCount;
  };
} // anonymous namespace


SCENARIO("Mesh ownership semantics")
{
  MockMesh* mock_mesh{ new MockMesh };
  REQUIRE(mock_mesh->count() == 1);

  WHEN("a Mesh is added to a Renderable using defaults")
  {
    THEN("the Mesh does not get deleted when the Renderable goes out of scope.")
    {
      NoDice::Renderable* renderable{ new NoDice::Renderable };
      renderable->add_mesh(mock_mesh);
      delete renderable;

      REQUIRE(mock_mesh->count() == 1);
    }
  }

  WHEN("a Mesh is added to a Renderable indicating ownership")
  {
    THEN("the Mesh gets deleted when the Renderable goes out of scope.")
    {
      NoDice::Renderable* renderable{ new NoDice::Renderable };
      renderable->add_mesh(mock_mesh, NoDice::Renderable::PointerOwnership::Owned);
      delete renderable;

      REQUIRE(mock_mesh->count() == 0);
    }
  }
}

SCENARIO("A Renderable is missing required attributes")
{
  NoDice::Renderable renderable;

  GIVEN("no Mesh has been added to the Renderable")
  {
    WHEN("the Renderable's draw() function is called")
    {
      THEN("a logic_error exception is thrown")
      {
        REQUIRE_THROWS_AS(renderable.draw(), std::logic_error);
      }
    }
  }

  GIVEN("a Renderable with a Mesh but no ShaderPipeline")
  {
    MockMesh mesh;
    renderable.add_mesh(&mesh);

    WHEN("the Renderable's draw() function is called")
    {
      THEN("a logic_error exception is thrown")
      {
        REQUIRE_THROWS_AS(renderable.draw(), std::logic_error);
      }
    }
  }
}
