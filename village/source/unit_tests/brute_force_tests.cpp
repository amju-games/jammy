#include <memory>
#include "catch.hpp"
#include "brute_force.h"

class my_game_object : public game_object
{
};

TEST_CASE("create, destroy", "[brute_force]")
{
  game_objects g;
  brute_force<my_game_object> bf;
  bf.set_game_objects(&g);
}

void check_size(int n)
{
  game_objects g;
  brute_force<my_game_object> bf;
  bf.set_game_objects(&g);

  REQUIRE(bf.broad_phase().size() == 0);

  for (int i = 0; i < n; i++)
  {
    g.push_back(std::make_shared<my_game_object>());
  }

  const int num_pairs = n * (n - 1) / 2;
  REQUIRE(bf.broad_phase().size() == num_pairs); 
}

TEST_CASE("check size of result", "[brute_force]")
{
  for (int i = 1; i <= 10; i++)
  {
    check_size(i);
  }
}

