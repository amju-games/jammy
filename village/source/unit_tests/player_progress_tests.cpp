#include "catch.hpp"
#include "player_progress.h"

TEST_CASE("initial progress", "[player_progress]")
{
  player_progress pp;
  
  REQUIRE(pp.get_score() == 0);
}

TEST_CASE("add to score", "[player_progress]")
{
  player_progress pp;

  pp.add_score(10);
  pp.add_score(20);

  REQUIRE(pp.get_score() == 30);
}

