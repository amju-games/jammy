#include <iostream>
#include "directory.h"
#include "input.h"
#include "globals.h"
#include "jammy_blend.h"
#include "player.h"
#include "sign.h"
#include "resources.h"
#include "universe.h"

#define NO_DECEL

const float MAX_IMMUNE_TIME = 3.f;
//const float DECELERATION = 0.5f;
const float FLAME_FRAME_TIME = 0.15f;

float PLAYER_SPEED = 20.f; 
float MAX_PLAYER_SPEED = 40.f; // TODO

player::player()
{
  m_is_collidable = true;

  m_sprite.set_image(resources().get<image>(get_data_dir() + "player1.png"));
  m_sprite.set_num_cells(3, 2);

  m_flames.set_image(resources().get<image>(get_data_dir() + "flames.png"));
  m_flames.set_num_cells(4, 6); // 4 frames * 6 directions
  m_flames.set_cell_time(FLAME_FRAME_TIME);

  m_pos = vec2(0, 0); 

  m_is_immune = false;
  m_immune_time = 0;
  m_lives = 5;

  m_anim_controller = std::make_unique<anim_controller_flying>();
  m_vel_controller = std::make_unique<vel_controller_flying>();
  m_vel_controller->set_max_speed(50.0f);

  set_up_anims();
}

void player::keep_immune()
{
  m_immune_time = MAX_IMMUNE_TIME;
  m_is_immune = true;
}

int player::lose_life()
{
  if (m_lives > 0)
  {
    m_lives--;
  }

  keep_immune();

  return m_lives;
}

static float flasher = 0;
static const float FLASH_PERIOD = .3f;

void player::draw(ref_image dest)
{
  if (m_is_immune)  
  {
    if (flasher > FLASH_PERIOD * .5f)
    {
      return;
    }
  }

  jammy_game_object::draw(dest);

  // Draw jet pac flame
  if (m_flame_on)
  {
    draw_sprite<jb_mask>(m_flames, dest);
  }
}

void player::set_up_anims()
{
  const float cell_time = 0.1f; // TODO
  m_anim_controller->set_anim_info(anim_state::idle_right, anim_info { 0, 0, true, cell_time });
  m_anim_controller->set_anim_info(anim_state::idle_left, anim_info { 3, 3, true, cell_time });
  m_anim_controller->set_anim_info(anim_state::face_right, anim_info { 0, 0, true, cell_time });
  m_anim_controller->set_anim_info(anim_state::face_left, anim_info { 3, 3, true, cell_time });
}

void player::update_anim(float dt)
{
  m_anim_controller->set_vel(get_vel());
  m_anim_controller->update(dt);

  const anim_info& ai = m_anim_controller->get_anim_info(); 

  m_sprite.set_cell_range(ai.m_cell_min, ai.m_cell_max);

  // Anim transitioner takes care of same/different cell range
  int cell = ai.m_cell_min; //m_anim_controller->get_cell();
  m_sprite.set_cell(cell);
}

void player::update_vel(float dt)
{
  auto& vc = get_vel_controller();
  vc.update(dt);
  set_vel(vc.get_vel());

  // Hmm, need this for firing laser.
  // TODO Get the dir from the current anim_info?
  if (m_vel.x > 0)
  {
    m_player_dir = player_dir::RIGHT;
  }
  else if (m_vel.x < 0)
  {
    m_player_dir = player_dir::LEFT;
  }
}

vel_controller& player::get_vel_controller()
{
  return *(m_vel_controller.get());
}

void player::update(float dt)
{
  if (m_is_immune)
  {
    flasher += dt;
    if (flasher > FLASH_PERIOD)
    {
      flasher = 0;
    }

    m_immune_time -= dt;
    if (m_immune_time <= 0)
    {
      m_immune_time = 0;
      m_is_immune = false;
    }
  }

  // This shouldn't be in here, it should be in a camera class
  set_cam_pos(m_pos); // TODO some elasticity

  update_vel(dt);
  update_anim(dt);
  jammy_game_object::update(dt); 

  m_flames.update(dt);
}

