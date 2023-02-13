#include <iostream>

#include "fps_counter.h"
#include "globals.h"
#include "jammy_blend.h"
#include "jammy_game_state.h"
#include "resources.h"
#include "string_utils.h"
#include "universe.h"

fps_counter the_fps_counter;

jammy_game_state::jammy_game_state()
{
  m_font_filename = "nice.font";
}

void jammy_game_state::on_active()
{
  m_time_in_state = 0;
  m_font.load(m_font_filename);
}

nice_font& jammy_game_state::get_font()
{
  return m_font;
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
  int y = PRETEND_SCREEN_H - 8;

  get_font().draw<jb_font_mask>(the_screen, 1, y, concat(
    std::to_string(the_fps_counter.get_fps()),
    "FPS  DRAW: ",
    the_game.get_draw_time().count(),
    "  UPDATE: ",
    the_game.get_update_time().count()
  ));
}

void jammy_game_state::update(float dt)
{
  the_fps_counter.update(dt);

  m_time_in_state += dt;
}

