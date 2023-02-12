#include "blit.h"
#include "directory.h"
#include "globals.h"
#include "jammy_blend.h"
#include "resources.h"
#include "splash_state.h"

splash_state::splash_state()
{
  m_image = resources().get<image>(get_data_dir() + "Splash_Screen.png");
}

static float flash = 0;
static const float FLASH_PERIOD = 1.f;

void splash_state::update(float dt) 
{
  flash += dt;
  if (flash > FLASH_PERIOD)
  {
    flash = 0;
  }
}

void splash_state::draw()
{
  blit<jb_overwrite>(m_image, the_screen, 0, 0);
  
  if (flash < FLASH_PERIOD * .5f)
  {
    get_font()->draw<jb_font_mask>(the_screen, 28, 45, "HIT SPACE TO START!");
  }
  get_font()->draw<jb_font_mask>(the_screen, 1, 120, "ARROW KEYS TO MOVE, ESC TO QUIT!");
}

bool splash_state::on_keyboard_action(const keyboard_action& ka)
{
  if (jammy_game_state::on_keyboard_action(ka))
  {
    return true;
  }

  auto [key, value](ka);
  if (value == button_value::down)
  {
    the_game.set_game_state(the_new_game_state);
    return true;
  }

  return false;
}

bool splash_state::on_game_controller_button_action(const game_controller_button_action& gcba) 
{
  if (jammy_game_state::on_game_controller_button_action(gcba))
  {
    return true;
  }

  auto [button, value](gcba);
  if (value == button_value::down)
  {
    the_game.set_game_state(the_new_game_state);
    return true;
  }

  return false;
}

