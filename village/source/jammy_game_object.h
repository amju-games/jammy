#pragma once

#include "game_object.h"
#include "sprite.h"
#include "universe.h"

class jammy_game_object : public game_object
{
public:
  // TODO load

  jammy_game_object();
  ~jammy_game_object();

  static void set_cam_pos(const vec2& pos);

  virtual void draw(ref_image dest);
  void update(float dt) override; 

  virtual std::string print() const;

  friend bool sprite_collision(
    jammy_game_object* jgo1, jammy_game_object* jgo2); 

protected:
  template<class BLENDER>
  void draw_sprite(const sprite& spr, ref_image dest) const
  {
    if (is_visible())
    {
      vec2 rel_pos = m_pos - s_cam_pos + CENTRE_SCREEN;
      spr.draw<BLENDER>(dest, rel_pos.x, rel_pos.y);
    }
  }

protected:
  sprite m_sprite;

  static vec2 s_cam_pos;
};

