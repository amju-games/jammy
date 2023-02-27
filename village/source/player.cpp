#include <iostream>
#include "directory.h"
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
  set_is_collidable(true);

  m_head.set_image(resources().get<image>(get_data_dir() + "player_heads.png"));
  m_head.set_num_cells(8, 4);
  m_head.set_cell_range(0, 15);
  m_head.set_cell_time(0.3f);

  m_reflect_helmet = std::make_shared<image_sphere_map>();
  m_reflect_helmet->add_child(
    resources().get<image>(get_data_dir() + "player_helmet_normals.png"));
  m_reflect_helmet->add_child(
    resources().get<image>(get_data_dir() + "player_helmet_normals.png")); // TODO

  sprite s;
  s.set_image(resources().get<image>(get_data_dir() + "player_flying_2.png"));
  s.set_num_cells(4, 2);
  set_sprite(s);

  m_flames.set_image(resources().get<image>(get_data_dir() + "flames.png"));
  m_flames.set_num_cells(4, 6); // 4 frames * 6 directions
  m_flames.set_cell_time(FLAME_FRAME_TIME);

  m_pos = vec2(0, 0); 

  m_is_immune = false;
  m_immune_time = 0;

  m_anim_controller_body_flying = std::make_unique<anim_controller_flying>();
  m_anim_controller_head = std::make_unique<anim_controller_flying>();

  m_vel_controller = std::make_unique<vel_controller_flying>();
  m_vel_controller->set_max_speed(50.0f);

  set_up_anims();
}

void player::keep_immune()
{
  m_immune_time = MAX_IMMUNE_TIME;
  m_is_immune = true;
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

  draw_sprite<jb_mask>(m_head, dest, vec2(6, 3));

//  vec2 helmet_pos = calc_rel_pos();
//  blit<alpha_blend>(m_reflect_helmet, dest, helmet_pos.x, helmet_pos.y);

  // Draw jet pac flame
  if (m_flame_on)
  {
    draw_sprite<jb_mask>(m_flames, dest);
  }
}

void player::set_up_anims()
{
  const float body_cell_time = 0.3f; // TODO CONFIG
  m_anim_controller_body_flying->set_anim_info(anim_state::idle_right, anim_info { 0, 3, true, body_cell_time });
  m_anim_controller_body_flying->set_anim_info(anim_state::idle_left, anim_info { 4, 7, true, body_cell_time });
  m_anim_controller_body_flying->set_anim_info(anim_state::face_right, anim_info { 0, 3, true, body_cell_time });
  m_anim_controller_body_flying->set_anim_info(anim_state::face_left, anim_info { 4, 7, true, body_cell_time });

  const float head_cell_time = 0.25f;
  m_anim_controller_head->set_anim_info(anim_state::idle_right, anim_info { 0, 15, true, head_cell_time });
  m_anim_controller_head->set_anim_info(anim_state::idle_left, anim_info { 16, 31, true, head_cell_time });

  // TODO blink but don't look around
  m_anim_controller_head->set_anim_info(anim_state::face_right, anim_info { 0, 0, true, head_cell_time });
  m_anim_controller_head->set_anim_info(anim_state::face_left, anim_info { 16, 16, true, head_cell_time });
  
  // Set initial states
  m_anim_controller_head->set_anim_state(anim_state::idle_right);
  m_anim_controller_body_flying->set_anim_state(anim_state::idle_right);
}

void player::update_head_anim(float dt)
{
  m_anim_controller_head->set_vel(get_vel());
  m_anim_controller_head->update(dt);

  int cell = m_anim_controller_head->get_cell();
  m_head.set_cell(cell);

#ifdef ANIM_DEBUG
const anim_info& ai = m_anim_controller_head->get_anim_info(); 
std::cout << "Head current state: " 
    << static_cast<int>(m_anim_controller_head->get_anim_state())
    << " min cell: " << ai.m_cell_min 
    << " max cell: " << ai.m_cell_max
    << " cell: " << cell << "\n";
#endif
}

void player::update_body_anim(float dt)
{
  m_anim_controller_body_flying->set_vel(get_vel());
  m_anim_controller_body_flying->update(dt);

  const anim_info& ai = m_anim_controller_body_flying->get_anim_info(); 
  get_sprite().set_cell_range(ai.m_cell_min, ai.m_cell_max);

  // Anim transitioner takes care of same/different cell range
  int cell = ai.m_cell_min; //m_anim_controller->get_cell();
  get_sprite().set_cell(cell);
}

void player::update_vel(float dt)
{
  auto& vc = get_vel_controller();

  vc.set_vel(get_vel());
  vc.set_acc(get_acc());
  vc.update(dt); // updates vel, set it back in player
  set_vel(vc.get_vel());
  set_acc(vc.get_acc());

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
  update_head_anim(dt);
  update_body_anim(dt);
  jammy_game_object::update(dt); 

  m_flames.update(dt);
}

