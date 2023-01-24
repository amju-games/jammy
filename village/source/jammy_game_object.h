#pragma once

#include "game_object.h"
#include "sprite.h"

class jammy_game_object : public game_object
{
public:
  // TODO load

  static void set_cam_pos(const vec2& pos);

  virtual void draw(ref_image dest);
  void update(float dt) override; 

  virtual std::string print() const;

  friend bool sprite_collision(
    jammy_game_object* jgo1, jammy_game_object* jgo2); 

protected:
  void draw_sprite(const sprite& spr, ref_image dest) const;

protected:
  sprite m_sprite;

  static vec2 s_cam_pos;
};

