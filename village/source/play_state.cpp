#include <iostream>
#include <bitset>

#include <cassert>
#include "blit.h"
#include "directory.h"
#include "game.h"
#include "globals.h"
#include "hq.h"
#include "human.h"
#include "human_list.h"
#include "jammy_blend.h"
#include "jammy_game_object.h"
#include "parallax_bg.h"
#include "player.h"
#include "player_bullet.h"
#include "play_state.h"
#include "resources.h"
#include "rock.h"
#include "string_utils.h"
#include "universe.h"

const int RADAR_X = 0;
const int RADAR_Y = 0;
const vec2 RADAR_CENTRE(RADAR_X + 8, RADAR_Y + 8);

float human_timer = 0;
int human_to_display = 0;

play_state::play_state()
{
  m_life_empty = resources().get<image>(get_data_dir() + "life_empty.png");
  m_life_full = resources().get<image>(get_data_dir() + "life_full.png");

  m_radar = resources().get<image>(get_data_dir() + "radar.png");
  m_blips.set_image(resources().get<image>(get_data_dir() + "blips.png"));
  m_blips.set_num_cells(3, 2);
}

void play_state::on_keyboard_action(const keyboard_action& ka) 
{
  auto [key, value](ka);
  // TODO Remappable controls
  if (key == ' ' && value == button_value::down)
  {
    the_level_manager.get_level().add_player_bullet();
  }
}

void play_state::on_game_controller_button_action(const game_controller_button_action& gcba) 
{
  auto [button, state] (gcba);
  // TODO Remappable controls
  if (button == 0 && state == button_value::down)
  {
    the_level_manager.get_level().add_player_bullet();
  }
}

void play_state::on_dir_button_action(const dir_button_action& dba)
{
  the_level_manager.get_level().get_player().get_vel_controller().on_dir_button_action(dba);
}

void play_state::on_joystick_action(const joystick_action& ja)
{
  the_level_manager.get_level().get_player().get_vel_controller().on_joystick_action(ja);
}

void play_state::on_active() 
{
  the_sound_player->play_wav(get_data_dir() + "sounds/sfx_sounds_powerup2.wav");
}

void play_state::on_deactive() 
{
}

void play_state::update(float dt)
{
  the_game.update_game_objects(dt); 

  the_level_manager.get_level().col_det();

  // Check for game over. Should be in player progress?
  player& p = the_level_manager.get_level().get_player();
  if (!p.is_immune() && p.get_num_lives() < 1)
  {
    the_game.set_game_state(the_game_over_state);
    the_sound_player->play_wav(get_data_dir() + "sounds/Shut_Down1.wav");
  }

  jammy_game_state::update(dt); // update fps counter
}

void play_state::draw_blip(jammy_game_object* h, int cell)
{
  vec2 d = h->get_pos() - the_level_manager.get_level().get_player().get_pos();
  const float MAX_DIST = 200.f;
  float dist_sq = squared_length(d);
  if (dist_sq < MAX_DIST * MAX_DIST)
  {
    vec2 pos = RADAR_CENTRE + d * (10.f / MAX_DIST);
    m_blips.draw_cell<jb_mask>(the_screen, cell, pos.x, pos.y);
  }
} 

void play_state::draw_radar()
{
  // Draw radar
  blit<jb_mask>(m_radar, the_screen, RADAR_X, RADAR_Y);
}
 
void play_state::draw_lives()
{
  int lives = the_level_manager.get_level().get_player().get_num_lives();
  for (int i = 0; i < lives; i++)
  {
    const int HEART_W = 10;
    blit<jb_mask>(m_life_full, the_screen, PRETEND_SCREEN_W - (i + 1) * HEART_W, 2);
  }
}
 
void play_state::draw() 
{
  game_objects* gos = the_game.get_game_objects();
  for (p_game_object& go : *gos)
  {
    jammy_game_object* jgo = dynamic_cast<jammy_game_object*>(go.get());
    assert(jgo);
    jgo->draw(the_screen);
  }

  draw_lives();
  
  // Draw score
  the_font.draw<jb_font_mask>(the_screen, 1, 1, std::to_string(the_level_manager.get_level().get_player().get_score()));

//  the_font.draw<jb_font_mask>(the_screen, 20, 8,  concat("POS: ", the_level_manager.get_level().get_player().get_pos()));
  the_font.draw<jb_font_mask>(the_screen, 20, 16, concat("VEL: ", the_level_manager.get_level().get_player().get_vel()));
  the_font.draw<jb_font_mask>(the_screen, 20, 24, concat("ACC: ", the_level_manager.get_level().get_player().get_acc()));

  jammy_game_state::draw();
}

