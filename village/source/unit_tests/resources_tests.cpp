#include "catch.hpp"
#include "image.h"
#include "resources.h"

TEST_CASE("get resource manager", "[resources]")
{
  auto& r = resources();
}

TEST_CASE("get a resource from manager", "[resources]")
{
  auto im = resources().get<image>("test.png");
  REQUIRE(im);
}


