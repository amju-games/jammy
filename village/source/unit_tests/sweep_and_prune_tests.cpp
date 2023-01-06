// "microlib" minimal C++ game utility library
// (c) Copyright 2017 Jason Colman

#include <memory>
#include "catch.hpp"
#include "game_object.h"
#include "sweep_and_prune.h"

namespace
{
class my_game_object : public game_object
{
};
}

using SAP1 = sweep_and_prune<1, my_game_object>;
using extent_vec = SAP1::extent_vec;

TEST_CASE("create, destroy sap", "[sweep_and_prune]")
{
  // 1-axis SAP. 
  SAP1 sap;
}

TEST_CASE("add extents", "[sweep_and_prune]")
{
  SAP1 sap;

  const int id = 42;
  const float min_value = 1.f;
  const float max_value = 2.f;

  sap.add_extents(0, id, min_value, max_value);

  // Same as:
  const bool IS_MIN = true;
  sap.add_extent(0, SAP1::extent{id, min_value, IS_MIN}); 
  sap.add_extent(0, SAP1::extent{id, max_value, !IS_MIN});
}

TEST_CASE("check one axis", "[sweep_and_prune]")
{
  SAP1 sap;
  extent_vec extents
  {
  };
  auto result = sap.one_axis(extents);
}


/*
TEST_CASE("update, is time to execute", "[message_queue]")
{
  reset_counter();
  message_queue mq;
  message msg([]() { s_counter++; }, 1.f);
  mq.add(msg);

  REQUIRE(s_counter == 0); 

  // Time has to be exceeded
  mq.update(1.f);
  REQUIRE(s_counter == 0); 
  mq.update(.01f);
  REQUIRE(s_counter == 1); 

  // Only executed once
  mq.update(1.f);
  REQUIRE(s_counter == 1); 
}
*/

