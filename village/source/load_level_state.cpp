#include "config_file.h"
#include "directory.h"
#include "globals.h"
#include "image_scale.h"
#include "jammy_blend.h"
#include "load_level_state.h"
#include "resources.h"

namespace
{
  const float MIN_TIME_IN_STATE = 1.f;
}

void load_level_state::on_active() 
{
  jammy_game_state::on_active();

  std::shared_ptr<config_file> config = resources().get<config_file>("config.txt");
  m_bg_colour = config->get_f_colour("::bg_colour").to_colour();
  
  // TODO: initialise loading level.
  // Call partial load each update; draw progress.
  // End state when level loaded

  the_level_manager.make_new_level();
  level& lev = the_level_manager.get_level(); 
  lev.set_level_num(the_player_progress.get_level());
  lev.load(); 
}

void load_level_state::update(float dt) 
{
  jammy_game_state::update(dt);
}

void load_level_state::draw() 
{
  // TODO Scrolling parallax bg
  the_screen->clear(m_bg_colour);

  jammy_game_state::draw();

  get_font().draw<jb_font_mask>(the_screen, 50, 20, "LEVEL"); // TODO Centred

  int level_num = the_player_progress.get_level();
  get_font()
    .set_scale(8.f)
    .draw<jb_font_mask>(the_screen, 50, 40, std::to_string(level_num))
    .set_scale(1.f);
}

bool load_level_state::on_keyboard_action(const keyboard_action& ka) 
{
  if (jammy_game_state::on_keyboard_action(ka))
  {
    return true;
  }

  auto [key, value](ka);
  if (   value == button_value::down 
      && get_time_in_state() > MIN_TIME_IN_STATE)
  {
    the_game.set_game_state(the_play_state);
    return true;
  }
  
  return false;
}

bool load_level_state::on_game_controller_button_action(const game_controller_button_action& gcba) 
{
  if (jammy_game_state::on_game_controller_button_action(gcba))
  {
    return true;
  }

  auto [button, value](gcba);
  if (   value == button_value::down 
      && get_time_in_state() > MIN_TIME_IN_STATE)
  {
    the_game.set_game_state(the_play_state);
    return true;
  }

  return false;
}


