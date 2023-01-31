#pragma once

#include "anim_controller.h"
#include "jammy_game_object.h"
#include "vel_controller.h"

class player : public jammy_game_object
{
public:
  player();

  void update(float dt) override;

  void draw(ref_image dest) override;

  // Shoot in direction of movement
  void shoot();

  void keep_immune();
 
  bool is_immune() const { return m_is_immune; }

  void add_human_saved() { m_humans_saved++; }

  int get_num_humans_saved() const { return m_humans_saved; }

  // For Defender-style firing, we need to know if we're facing left or right
  // TODO This could go in Walker perhaps?
  enum class player_dir { LEFT, RIGHT };
  player_dir get_player_dir() const { return m_player_dir; }

  vel_controller& get_vel_controller();

private:
  void set_up_anims();
  void update_anim(float dt);
  void update_vel(float dt);

  // TODO: set_activity, switch between different vel/anim controllers

private:
  // TODO Multiple controllers for different activities
  std::unique_ptr<anim_controller> m_anim_controller;
  std::unique_ptr<vel_controller> m_vel_controller;

  bool m_is_immune = false;
  float m_immune_time = 0;
  int m_humans_saved = 0;
  player_dir m_player_dir = player_dir::RIGHT;

  sprite m_flames;
  bool m_flame_on = false;
};

