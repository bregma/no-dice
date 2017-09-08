/**
 * @file test_mesh.cpp
 * @brief Unit tests for the no- dice mesh module.
 *
 * Copyright 2017 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
#include "catch/catch.hpp"
#include "nodice/mesh.h"
#include "nodice/shaderpipeline.h"


namespace 
{
  class MockMesh
  : public NoDice::Mesh
  {
  private:
    ~MockMesh() {}

    void store_vertex_data(float const*) override { }
    void reset_vertex_data() override { }
    void store_index_data(std::uint16_t const*) override { }
    void reset_index_data() override { }
    void activate() override { }
    bool is_active_p() const override { return true; }
    void deactivate() override { }
    void draw_direct() override { ++draw_direct_count; }
    void draw_indexed() override { ++draw_indexed_count; }

  public:
    int draw_direct_count = 0;
    int draw_indexed_count = 0;
  };


  class MockShaderPipeline
  : public NoDice::ShaderPipeline
  {
  public:
    MockShaderPipeline(NoDice::ShaderPipeline::Id id) : NoDice::ShaderPipeline(id) {}
    ~MockShaderPipeline() {}

    void attach_stage_p(NoDice::ShaderStage const&) override { }
    void link_p() override { }
    void activate_p() override { }
    bool is_pipeline_active() const override { return true; }
    void deactivate_p() override { }
    void set_uniform_p(const std::string&, const NoDice::mat4&) override { }
    void set_attribute_p(const std::string&, int, int, void const*) override { ++this->set_attribute_count; }

  public:
    int set_attribute_count = 0;
  };
} // anonymous namespace


SCENARIO("default-constructed Mesh")
{
  WHEN("a Mesh is default-constructed")
  {
    NoDice::Mesh* mesh = new MockMesh;

    THEN("drawing should not crash")
    {
      REQUIRE(true);
    }
  }
}


SCENARIO("a Mesh with vertex attributes added")
{
  MockShaderPipeline pipeline(1);

  NoDice::Mesh* mesh = new MockMesh;
  float verts[] = { 1.0f, 1.0f };
  mesh->add_vertex_data(1,
                        {{NoDice::Mesh::VertexTargetType::Position, 2, NoDice::Mesh::VertexUsageHint::Static}},
                        verts);

  WHEN("no indexes are set and a draw call is made")
  {
    THEN("the direct_draw() call is made")
    {
      mesh->draw(pipeline);
      REQUIRE(static_cast<MockMesh*>(mesh)->draw_direct_count == 1);
    }
  }
  WHEN("indexes are set and a draw call is made")
  {
    mesh->add_index_data(1, nullptr);
    THEN("the indexed_draw() call is made")
    {
      mesh->draw(pipeline);
      REQUIRE(static_cast<MockMesh*>(mesh)->draw_indexed_count == 1);
      REQUIRE(pipeline.set_attribute_count == 1);
    }
  }
}

