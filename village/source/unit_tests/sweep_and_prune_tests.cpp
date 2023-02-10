// "microlib" minimal C++ game utility library
// (c) Copyright 2017 Jason Colman

#include <memory>
#include "catch.hpp"
#include "double_dispatcher.h"
#include "game_object.h"
#include "sweep_and_prune.h"

namespace
{
class my_game_object : public game_object
{
public:
  // Broad phase ignores non-collidable game objects
  my_game_object() { set_is_collidable(true); }
};

// Mock double disptcher, because brute_force prunes pairs with no handler.
class dd : public double_dispatcher_interface<my_game_object>
{
public:
  bool dispatch(my_game_object* obj1, my_game_object* obj2) const override { return true; }
  bool has_handler(my_game_object* obj1, my_game_object* obj2) const override { return true; }
};
}

using SAP1 = sweep_and_prune<1, my_game_object, dd>;
using SAP2 = sweep_and_prune<2, my_game_object, dd>;
using extent_vec = SAP1::extent_vec;

TEST_CASE("create, destroy sap", "[sweep_and_prune]")
{
  // 1-axis SAP. 
  SAP1 sap;
  game_objects g;
  sap.set_game_objects(&g);
}

TEST_CASE("collision_pair order", "[collision_pair]")
{
  my_game_object go1;
  my_game_object go2;
  SAP1 sap;

  {
    auto cp = sap.make_coll_pair(&go1, &go2);
    REQUIRE(cp.first->get_id() < cp.second->get_id());
  }

  auto cp2 = sap.make_coll_pair(&go2, &go1);
  REQUIRE(cp2.first->get_id() < cp2.second->get_id());
}

TEST_CASE("add extents", "[sweep_and_prune]")
{
  SAP1 sap;

  const int id = 42;
  const float min_value = 1.f;
  const float max_value = 2.f;
  my_game_object go;

  sap.add_extents(0, &go, min_value, max_value);

  // Same as:
  const bool IS_MIN = true;
  sap.add_extent(0, SAP1::extent{&go, min_value, IS_MIN}); 
  sap.add_extent(0, SAP1::extent{&go, max_value, !IS_MIN});
}

TEST_CASE("get x extents for one game object", "[sweep_and_prune]")
{
  SAP2 sap;
  my_game_object go;
  go.set_pos(vec2(11, 22));
  go.set_size(vec2(5, 6));
  auto [ e1, e2 ] = sap.make_extents_for_game_object_for_axis(&go, 0);
  REQUIRE(e1.go == &go);
  REQUIRE(e1.pos == 11);
  REQUIRE(e1.is_min);
  REQUIRE(e2.go == &go);
  REQUIRE(e2.pos == 16);
  REQUIRE_FALSE(e2.is_min);
}

TEST_CASE("get y extents for one game object", "[sweep_and_prune]")
{
  SAP2 sap;
  my_game_object go;
  go.set_pos(vec2(11, 22));
  go.set_size(vec2(5, 6));
  auto [ e1, e2 ] = sap.make_extents_for_game_object_for_axis(&go, 1);
  REQUIRE(e1.go == &go);
  REQUIRE(e1.pos == 22);
  REQUIRE(e1.is_min);
  REQUIRE(e2.go == &go);
  REQUIRE(e2.pos == 28);
  REQUIRE_FALSE(e2.is_min);
}

TEST_CASE("populate extents for two game objects", "[sweep_and_prune]")
{
  SAP2 sap;
  auto go1 = std::make_shared<my_game_object>();
  go1->set_pos(vec2(11, 22));
  go1->set_size(vec2(5, 6));
  auto go2 = std::make_shared<my_game_object>();
  go2->set_pos(vec2(33, 44));
  go2->set_size(vec2(7, 8));

  sap.populate_extents( { go1, go2 } );

  auto ev = sap.get_extent_vec(0);
  REQUIRE(ev.size() == 2 * 2);
  REQUIRE(ev[0].go == go1.get());
  REQUIRE(ev[0].pos == go1->get_pos().x);
  REQUIRE(ev[0].pos == 11);
  REQUIRE(ev[0].is_min);
  REQUIRE(ev[1].go == go1.get());
  REQUIRE(ev[1].pos == go1->get_pos().x + go1->get_size().x);
  REQUIRE(ev[1].pos == 16);
  REQUIRE_FALSE(ev[1].is_min);
  REQUIRE(ev[2].go == go2.get());
  REQUIRE(ev[2].pos == go2->get_pos().x);
  REQUIRE(ev[2].pos == 33);
  REQUIRE(ev[2].is_min);
  REQUIRE(ev[3].go == go2.get());
  REQUIRE(ev[3].pos == go2->get_pos().x + go2->get_size().x);
  REQUIRE(ev[3].pos == 40);
  REQUIRE_FALSE(ev[3].is_min);
  
  ev = sap.get_extent_vec(1);
  REQUIRE(ev.size() == 2 * 2);
  REQUIRE(ev[0].go == go1.get());
  REQUIRE(ev[0].pos == go1->get_pos().y);
  REQUIRE(ev[0].pos == 22);
  REQUIRE(ev[0].is_min);
  REQUIRE(ev[1].go == go1.get());
  REQUIRE(ev[1].pos == go1->get_pos().y + go1->get_size().y);
  REQUIRE(ev[1].pos == 28);
  REQUIRE_FALSE(ev[1].is_min);
  REQUIRE(ev[2].go == go2.get());
  REQUIRE(ev[2].pos == go2->get_pos().y);
  REQUIRE(ev[2].pos == 44);
  REQUIRE(ev[2].is_min);
  REQUIRE(ev[3].go == go2.get());
  REQUIRE(ev[3].pos == go2->get_pos().y + go2->get_size().y);
  REQUIRE(ev[3].pos == 52);
  REQUIRE_FALSE(ev[3].is_min);
}

TEST_CASE("update extents", "[sweep_and_prune]")
{
  SAP2 sap;
  auto go1 = std::make_shared<my_game_object>();
  go1->set_pos(vec2(11, 22));
  go1->set_size(vec2(5, 6));
  auto go2 = std::make_shared<my_game_object>();
  go2->set_pos(vec2(33, 44));
  go2->set_size(vec2(7, 8));

  sap.populate_extents( { go1, go2 } );

  go1->set_pos(vec2(55, 66));
 
  sap.update_extents();

  auto ev = sap.get_extent_vec(0);

  // Updated
  REQUIRE(ev.size() == 2 * 2);
  REQUIRE(ev[0].go == go1.get());
  REQUIRE(ev[0].pos == 55);
  REQUIRE(ev[0].is_min);
  REQUIRE(ev[1].go == go1.get());
  REQUIRE(ev[1].pos == 60);
  REQUIRE_FALSE(ev[1].is_min);

  // Unchanged
  REQUIRE(ev[2].go == go2.get());
  REQUIRE(ev[2].pos == 33);
  REQUIRE(ev[2].is_min);
  REQUIRE(ev[3].go == go2.get());
  REQUIRE(ev[3].pos == 40);
  REQUIRE_FALSE(ev[3].is_min);
   
}

TEST_CASE("check one axis", "[sweep_and_prune]")
{
  SAP1 sap;
  auto go1 = std::make_shared<my_game_object>();
  auto go2 = std::make_shared<my_game_object>();
  extent_vec extents
  {
    { go1.get(), 5, true },
    { go1.get(), 10, false },
    { go2.get(), 8, true },
    { go2.get(), 12, false }
  };
  SAP1::collision_vec result;
  sap.one_axis(extents, result, dd());

  // Swapped
  REQUIRE(extents[1].pos == 8);
  REQUIRE(extents[2].pos == 10);

  REQUIRE(result.size() == 1);
  REQUIRE(result[0].first == go1.get());
  REQUIRE(result[0].second == go2.get());
}


