#include "catch.hpp"
#include "level_manager.h"

TEST_CASE("get current level", "[level_manager]")
{
  level_manager lm;
  level& lev = lm.get_level();
}

