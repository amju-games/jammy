#include <memory>
#include "catch.hpp"
#include "brute_force.h"
#include "double_dispatcher.h"

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

TEST_CASE("create, destroy", "[brute_force]")
{
  game_objects g;
  brute_force<my_game_object, dd> bf;
  bf.set_game_objects(&g);
}

void check_size(int n)
{
  game_objects g;
  brute_force<my_game_object, dd> bf;
  bf.set_game_objects(&g);

  REQUIRE(bf.broad_phase(dd()).size() == 0);

  for (int i = 0; i < n; i++)
  {
    g.push_back(std::make_shared<my_game_object>());
  }

  const int num_pairs = n * (n - 1) / 2;
  REQUIRE(bf.broad_phase(dd()).size() == num_pairs); 
}

TEST_CASE("check size of result", "[brute_force]")
{
  for (int i = 1; i <= 10; i++)
  {
    check_size(i);
  }
}

