#include "attract_state.h"
#include "config_file.h"
#include "globals.h"
#include "resources.h"

void attract_state::on_active()
{
  std::shared_ptr<config_file> config = resources().get<config_file>("config.txt");
  m_timeout = config->get_float("attract_state::m_timeout");
}

void attract_state::update(float dt) 
{
  play_state_base::update(dt);

  if (get_time_in_state() > m_timeout)
  {
    the_game.set_game_state(the_splash_state); // TODO make sure the states cycle
  }
}

bool attract_state::on_keyboard_action(const keyboard_action& ka) 
{
  if (play_state_base::on_keyboard_action(ka))
  {
    return true;
  }

  the_game.set_game_state(the_splash_state);
  return true;
}

bool attract_state::on_game_controller_button_action(const game_controller_button_action& gcba) 
{
  if (play_state_base::on_game_controller_button_action(gcba))
  {
    return true;
  }

  the_game.set_game_state(the_splash_state);
  return true;
}

