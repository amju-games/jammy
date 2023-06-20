#include "config_file.h"
#include "directory.h"
#include "globals.h"
#include "image_scale.h"
#include "jammy_blend.h"
#include "load_level_state.h"
#include "resources.h"

namespace
{
  const float MIN_TIME_IN_STATE = 0.f;
}

void load_level_state::on_active() 
{
  jammy_game_state::on_active();

  std::shared_ptr<config_file> config = resources().get<config_file>("config.txt");
  m_bg_colour = config->get_f_colour("::bg_colour").to_colour();
  m_level_x = config->get_int("load_level_state::m_level_x");
  m_level_y = config->get_int("load_level_state::m_level_y");
  m_level_scale = config->get_float("load_level_state::m_level_scale");
  m_num_x = config->get_int("load_level_state::m_num_x");
  m_num_y = config->get_int("load_level_state::m_num_y");
  m_num_scale = config->get_float("load_level_state::m_num_scale");
  
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

  get_font()
    .set_scale(m_level_scale)
    .draw<jb_font_mask>(the_screen, m_level_x, m_level_y, "LEVEL"); // TODO Centred

  int level_num = the_player_progress.get_level();
  std::string s = std::to_string(level_num);
  if (level_num < 10) s = "0" + s; // TODO func to add leading zeroes

  get_font()
    .set_scale(m_num_scale)
    .draw<jb_font_mask>(the_screen, m_num_x, m_num_y, s)
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


