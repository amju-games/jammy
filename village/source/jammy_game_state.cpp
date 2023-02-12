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

bool jammy_game_state::on_keyboard_action(const keyboard_action& ka)
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
    the_game.get_game_state()->on_deactive(); 
    the_game.get_game_state()->on_active(); // reload configs
    return true;
  };

  return false;
} 

void jammy_game_state::draw() 
{
  the_font.draw<jb_font_mask>(the_screen, 20, 2, std::to_string(the_fps_counter.get_fps()) + "FPS");
  the_font.draw<jb_font_mask>(the_screen, 20, 10, "DRAW:   " + std::to_string(the_game.get_draw_time().count()));
  the_font.draw<jb_font_mask>(the_screen, 20, 18, "UPDATE: " + std::to_string(the_game.get_update_time().count()));
}

void jammy_game_state::update(float dt)
{
  the_fps_counter.update(dt);

  m_time_in_state += dt;
}

