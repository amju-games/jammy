#include "directory.h"
#include "globals.h"
#include "image_scale.h"
#include "jammy_blend.h"
#include "load_level_state.h"
#include "resources.h"

namespace
{
  const float MIN_TIME_IN_STATE = 3.f;
}

load_level_state::load_level_state()
{
  p_image font_image = resources().get<image>(get_data_dir() + "font1 - magenta.png");
  // Scale decorator
  const float scale = 4.f;
  m_big_font.set_image(
    std::make_shared<image_scale>(
      font_image,
      //std::make_shared<image_colour_xform>( // hmm, unfortunately making the magenta also black
      //  font_image, 
      //  alg3::vec4(0, 0, 0, 1)), 
      scale));
  m_big_font.set_num_cells(16, 4);

}

void load_level_state::on_active() 
{
  jammy_game_state::on_active();

  the_screen->clear(colour(0xff, 0x80, 0));

  // TODO: initialise loading level.
  // Call partial load each update; draw progress.
  // End state when level loaded

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
  jammy_game_state::draw();

  int level_num = the_player_progress.get_level();
  m_big_font.draw<jb_font_mask>(the_screen, 20, 2, "LEVEL " + std::to_string(level_num));
}

void load_level_state::on_keyboard_action(const keyboard_action& ka) 
{
  jammy_game_state::on_keyboard_action(ka);

  auto [key, value](ka);
  if (value == button_value::down && get_time_in_state() > MIN_TIME_IN_STATE)
  {
    the_game.set_game_state(the_play_state);
  }
}

void load_level_state::on_game_controller_button_action(const game_controller_button_action& gcba) 
{
  jammy_game_state::on_game_controller_button_action(gcba);

  auto [button, value](gcba);
  if (value == button_value::down && get_time_in_state() > MIN_TIME_IN_STATE)
  {
    the_game.set_game_state(the_play_state);
  }
}


