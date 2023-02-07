#include <iostream>

#include "fps_counter.h"
#include "globals.h"
#include "jammy_blend.h"
#include "jammy_game_state.h"
#include "resources.h"

fps_counter the_fps_counter;

void jammy_game_state::on_active()
{
  m_time_in_state = 0;
}

void jammy_game_state::on_keyboard_action(const keyboard_action& ka)
{
  // Hard coded debug commands
  auto [key, state](ka);
  switch (key)
  {
  case 'R':
    [[fallthrough]]
    ;
  case 'r':
std::cout << "HOT RELOAD!\n";
    resources().reload();
    break;
  };
} 

void jammy_game_state::draw() 
{
  the_font.draw<jb_font_mask>(the_screen, 20, 2, std::to_string(the_fps_counter.get_fps()) + "FPS");
}

void jammy_game_state::update(float dt)
{
  the_fps_counter.update(dt);

  m_time_in_state += dt;
}

