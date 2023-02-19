#include "catch.hpp"
#include "rng.h"

namespace
{
  const float NUM_ITERS = 1000;
}

TEST_CASE("Numbers in expected range - float 0,n", "[rng]")
{
  rng r;
  for (int i = 0; i < NUM_ITERS; i++)
  {
    float f = r.rand(1.f);
    REQUIRE(f >= 0.f);
    REQUIRE(f < 1.f);
  }
}


TEST_CASE("Numbers in expected range - float m,n", "[rng]")
{
  rng r;
  for (int i = 0; i < NUM_ITERS; i++)
  {
    float f = r.rand(1.f, 2.f);
    REQUIRE(f >= 1.f);
    REQUIRE(f < 2.f);
  }
}


TEST_CASE("Numbers in expected range - int 0,n", "[rng]")
{
  rng r;
  for (int i = 0; i < NUM_ITERS; i++)
  {
    int n = r.rand(10);
    REQUIRE(n >= 0);
    REQUIRE(n < 10);
  }
}


TEST_CASE("Numbers in expected range - int m,n", "[rng]")
{
  rng r;
  for (int i = 0; i < NUM_ITERS; i++)
  {
    int n = r.rand(10, 20);
    REQUIRE(n >= 10);
    REQUIRE(n < 20);
  }
}


