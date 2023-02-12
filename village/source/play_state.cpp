#include <iostream>
#include <bitset>

#include <cassert>
#include "blit.h"
#include "directory.h"
#include "game.h"
#include "globals.h"
#include "hud.h"
#include "jammy_blend.h"
#include "jammy_game_object.h"
#include "player.h"
#include "player_bullet.h"
#include "play_state.h"
#include "resources.h"
#include "string_utils.h"
#include "universe.h"

void play_state_base::draw() 
{
  draw_game_objects();
  
  // Explicitly call this from subclass draw() 
  //jammy_game_state::draw(); // fps
}

void play_state::update(float dt)
{ 
  play_state_base::update(dt);

  check_for_game_over();
  
  check_for_level_completed(); 
}

void play_state_base::update(float dt)
{
  jammy_game_state::update(dt);

  the_game.update_game_objects(dt); 

  the_level_manager.get_level().col_det();
}

void play_state_base::draw_game_objects()
{
  game_objects* gos = the_game.get_game_objects();
  for (p_game_object& go : *gos)
  {
    jammy_game_object* jgo = dynamic_cast<jammy_game_object*>(go.get());
    assert(jgo);
    jgo->draw(the_screen);
  }
}
 
bool play_state::on_keyboard_action(const keyboard_action& ka) 
{
  auto [key, value](ka);
  // TODO Remappable controls
  if (key == ' ' && value == button_value::down)
  {
    the_level_manager.get_level().add_player_bullet();
    return true;
  }
  return false;
}

bool play_state::on_game_controller_button_action(const game_controller_button_action& gcba) 
{
  auto [button, state] (gcba);
  // TODO Remappable controls
  if (button == 0 && state == button_value::down)
  {
    the_level_manager.get_level().add_player_bullet();
    return true;
  }
  return false;
}

bool play_state::on_dir_button_action(const dir_button_action& dba)
{
  the_level_manager.get_level().get_player().get_vel_controller().on_dir_button_action(dba);
  return true;
}

bool play_state::on_joystick_action(const joystick_action& ja)
{
  the_level_manager.get_level().get_player().get_vel_controller().on_joystick_action(ja);
  return true;
}

void play_state::on_active() 
{
  the_sound_player->play_wav(get_data_dir() + "sounds/sfx_sounds_powerup2.wav");
}

void play_state::on_deactive() 
{
}

void play_state::check_for_game_over()
{
  // Check for game over. Should be in player progress?
  if (the_player_progress.get_lives() < 1)
  {
    the_level_manager.get_level().get_player().set_is_visible(false); 
    
    the_game.set_game_state(the_game_over_state);
    the_sound_player->play_wav(get_data_dir() + "sounds/Shut_Down1.wav");
  }
}

void play_state::go_to_next_level()
{
  int level = the_player_progress.get_level();
  the_player_progress.set_level(level + 1); // TODO level hopping 

  // TODO make player immune, set timer to go to next level state
  the_game.set_game_state(the_load_level_state);
}

void play_state::check_for_level_completed()
{
  if (the_level_manager.get_level().is_level_completed() &&
      the_player_progress.get_lives() > 0)
  {
    go_to_next_level();
  }
}

void play_state::draw_hud()
{
  static hud h;
  h.draw();
}

void play_state::draw()
{
  play_state_base::draw();
  draw_hud();

  jammy_game_state::draw(); // timing info
}


