#include "auto_repeat_suppressor.h"
#include "catch.hpp"

TEST_CASE("suppress repeated key down events", "[auto_repeat_suppressor]")
{
  auto_repeat_suppressor ars;
  keyboard_action repeated('a', button_value::down); 
  REQUIRE(ars.is_new_event(repeated)); 
  REQUIRE_FALSE(ars.is_new_event(repeated)); 
}

TEST_CASE("another key is not suppressed", "[auto_repeat_suppressor]")
{
  auto_repeat_suppressor ars;
  keyboard_action repeated('a', button_value::down); 
  REQUIRE(ars.is_new_event(repeated)); 
  REQUIRE_FALSE(ars.is_new_event(repeated)); 
  REQUIRE(ars.is_new_event(keyboard_action('b', button_value::down))); 
}

TEST_CASE("suppression reset after a key up event", "[auto_repeat_suppressor]")
{
  auto_repeat_suppressor ars;
 
  keyboard_action repeated('a', button_value::down); 
  REQUIRE(ars.is_new_event(repeated)); 
  REQUIRE_FALSE(ars.is_new_event(repeated)); 
  
  REQUIRE(ars.is_new_event(keyboard_action('a', button_value::up)));

  REQUIRE(ars.is_new_event(repeated)); 
}

