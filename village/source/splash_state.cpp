#include "blit.h"
#include "config_file.h"
#include "directory.h"
#include "globals.h"
#include "jammy_blend.h"
#include "resources.h"
#include "splash_state.h"

splash_state::splash_state()
{
}

void splash_state::on_active()
{
  jammy_game_state::on_active();

  std::shared_ptr<config_file> config = resources().get<config_file>("config.txt");
  m_time_to_next_state = config->get_int("splash_state::m_time_to_next_state");
  
  m_image = resources().get<image>("Splash_Screen.png");
}

static float flash = 0;
static const float FLASH_PERIOD = 1.f;

void splash_state::update(float dt) 
{
  jammy_game_state::update(dt);

  flash += dt;
  if (flash > FLASH_PERIOD)
  {
    flash = 0;
  }

  if (get_time_in_state() > m_time_to_next_state)
  {
    the_game.set_game_state(the_attract_state);
  }
}

void splash_state::draw()
{
  blit<jb_overwrite>(m_image, the_screen, 0, 0);
  
  if (flash < FLASH_PERIOD * .5f)
  {
    get_font().draw<jb_font_mask>(the_screen, 28, 45, "HIT SPACE TO START!");
  }
  get_font().draw<jb_font_mask>(the_screen, 1, 120, "ARROW KEYS TO MOVE, ESC TO QUIT!");
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

