#include "catch.hpp"
#include "vel_controller.h"

namespace
{
  const float some_dt_value = 0.1f;
  const float some_max_speed = 7.0f;
}

TEST_CASE("initialised vel_controller outputs zero acc and vel", "[vel_controller_buttons_no_accel]")
{
  vel_controller_buttons_no_accel vc;

  REQUIRE(vc.get_vel() == vec2(0, 0));
  REQUIRE(vc.get_acc() == vec2(0, 0));

  vc.update(some_dt_value);

  REQUIRE(vc.get_vel() == vec2(0, 0));
  REQUIRE(vc.get_acc() == vec2(0, 0));
}

TEST_CASE("right button outputs expected vel", "[vel_controller_buttons_no_accel]")
{
  vel_controller_buttons_no_accel vc;
 
  vc.on_dir_button_action(dir_button_action(dir_button_name::right, button_value::down));

  REQUIRE(vc.get_vel() == vec2(0, 0));

  vc.update(some_dt_value); // must update to see change in vel

  REQUIRE(vc.get_vel() == vec2(1, 0)); // default max speed is 1.
}

TEST_CASE("max_speed scales velocity vec length", "[vel_controller_buttons_no_accel]")
{
  vel_controller_buttons_no_accel vc;
 
  vc.set_max_speed(some_max_speed);
 
  vc.on_dir_button_action(dir_button_action(dir_button_name::right, button_value::down));

  vc.update(some_dt_value);

  REQUIRE(vc.get_vel() == vec2(some_max_speed, 0)); // vel vec is scaled by max speed
}

TEST_CASE("left button outputs expected vel", "[vel_controller_buttons_no_accel]")
{
  vel_controller_buttons_no_accel vc;
  vc.on_dir_button_action(dir_button_action(dir_button_name::left, button_value::down));
  vc.update(some_dt_value);
  REQUIRE(vc.get_vel() == vec2(-1, 0));
}

TEST_CASE("left and right button outputs expected vel", "[vel_controller_buttons_no_accel]")
{
  vel_controller_buttons_no_accel vc;
  vc.on_dir_button_action(dir_button_action(dir_button_name::left, button_value::down));
  vc.on_dir_button_action(dir_button_action(dir_button_name::right, button_value::down));
  vc.update(some_dt_value);
  REQUIRE(vc.get_vel() == vec2(0, 0));
}

TEST_CASE("joystick input outputs expected vel", "[vel_controller_flying]")
{
}


