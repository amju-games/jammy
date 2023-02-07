#include "anim_controller.h"
#include "catch.hpp"

namespace
{
  const float some_dt = 0.1f;
  const float some_cell_time = 0.1f;
}

TEST_CASE("initial anim state is idle", "[anim_controller_flying]")
{
  anim_controller_flying ac;
  REQUIRE(ac.get_anim_state() == anim_state::idle);
}

TEST_CASE("anim state corresponds to x vel", "[anim_controller_flying]")
{
  anim_controller_flying ac;

  ac.set_vel(vec2(-1, 0));
  ac.update(some_dt);

  REQUIRE(ac.get_anim_state() == anim_state::face_left);
}

TEST_CASE("store anim infos", "[anim_controller_stores_anim_info]")
{
  anim_controller_flying ac;

  ac.set_anim_info(anim_state::idle, anim_info { 0, 1, true, some_cell_time });
}

TEST_CASE("get idle anim info for initialised ac", "[anim_controller_flying]")
{
  anim_controller_flying ac;
  
  ac.set_anim_info(anim_state::idle, anim_info { 100, 101, true, some_cell_time });

  ac.update(some_dt);

  REQUIRE(ac.get_anim_state() == anim_state::idle);

  const anim_info& ai = ac.get_anim_info();
  REQUIRE(ai.m_cell_min == 100);
}

TEST_CASE("no transition between two states", "[anim_controller_flying]")
{
  anim_controller_flying ac;
  
  ac.set_anim_info(anim_state::face_left, anim_info { 100, 101, true, some_cell_time });
  ac.set_anim_info(anim_state::face_right, anim_info { 102, 103, true, some_cell_time });

  REQUIRE_FALSE(ac.has_transition(anim_state::face_left, anim_state::face_right));
}


