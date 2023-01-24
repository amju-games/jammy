#pragma once

#include "game_object.h"
#include "sprite.h"

class jammy_game_object : public game_object
{
public:
  // TODO load

  bool is_alive() const { return m_is_alive; }
  void set_is_alive(bool alive) { m_is_alive = alive; }

  static void set_cam_pos(const vec2& pos);

  virtual void draw(ref_image dest);
  void update(float dt) override; 

  friend bool sprite_collision(
    jammy_game_object* jgo1, jammy_game_object* jgo2); 

protected:
  void draw_sprite(const sprite& spr, ref_image dest) const;

protected:
  sprite m_sprite;

  // If not alive, we don't draw, update, or consider in collisions
  bool m_is_alive = true; 

  static vec2 s_cam_pos;
};

