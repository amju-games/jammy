#include "directory.h"
#include "globals.h"
#include "image_scale.h"
#include "jammy_blend.h"
#include "load_level_state.h"
#include "resources.h"

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
  the_screen->clear(colour(0xff, 0x80, 0));

  the_play_state->get_level().load(); // TODO for so many reasons
}

void load_level_state::update(float dt) 
{
}

void load_level_state::draw() 
{
  int level_num = 1; // TODO
  m_big_font.draw<jb_font_mask>(the_screen, 20, 2, "LEVEL " + std::to_string(level_num));
}

void load_level_state::on_keyboard_action(const keyboard_action& ka) 
{
  auto [key, value](ka);
  if (value == button_value::down)
  {
    the_game.set_game_state(the_play_state);
  }
}

void load_level_state::on_game_controller_button_action(const game_controller_button_action& gcba) 
{
  auto [button, value](gcba);
  if (value == button_value::down)
  {
    the_game.set_game_state(the_play_state);
  }
}


