/**
 * @file test_config.cpp
 * @brief Unit tests for the nodice/config module.
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
#include "nodice/config.h"


SCENARIO("basic command line options handling")
{
  WHEN("no command-line options are passed")
  {
    char* argv[] = { (char*)"no-dice" };
    int argc = sizeof(argv) / sizeof(char*);
    NoDice::Config config(argc, argv);
    THEN("sensible defaults are set")
    {
      REQUIRE(config.is_debug_mode() == false);
      REQUIRE(config.is_fullscreen() == false);
      REQUIRE(config.board_size() == 8);
    }
  }

  WHEN("the -d switch is passed")
  {
    char* argv[] = { (char*)"no-dice", (char*)"-d" };
    int argc = sizeof(argv) / sizeof(char*);
    NoDice::Config config(argc, argv);
    THEN("debug mode is configured")
    {
      REQUIRE(config.is_debug_mode() == true);
    }
  }

  WHEN("the -f switch is passed")
  {
    char* argv[] = { (char*)"no-dice", (char*)"-f" };
    int argc = sizeof(argv) / sizeof(char*);
    NoDice::Config config(argc, argv);
    THEN("full-screen mode is configured")
    {
      REQUIRE(config.is_fullscreen() == true);
    }
  }
}


SCENARIO("asset searching")
{
  GIVEN("a default-constructed Config")
  {
    char* argv[] = { (char*)"no-dice" };
    int argc = sizeof(argv) / sizeof(char*);
    NoDice::Config config(argc, argv);

    THEN("the asset search path containts at least one item")
    {
      REQUIRE(config.asset_search_path().size() > 0);
    }
  }
}

