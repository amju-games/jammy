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
  static const vec2& get_cam_pos();

  virtual void draw(ref_image dest);
  void update(float dt) override; 

  void set_sprite(const sprite&);
  sprite& get_sprite();

  virtual std::string print() const;

  friend bool sprite_collision(
    jammy_game_object* jgo1, jammy_game_object* jgo2); 

protected:
  vec2 calc_rel_pos(const vec2& offset = vec2(0, 0)) const
  {
    vec2 rel_pos = get_pos() - s_cam_pos + CENTRE_SCREEN + offset;
    return rel_pos;
  }

  template<class BLENDER>
  void draw_sprite(const sprite& spr, ref_image dest, const vec2& offset = vec2(0, 0)) const
  {
    if (is_visible())
    {
      vec2 rel_pos = calc_rel_pos(offset);
      spr.draw<BLENDER>(dest, rel_pos.x, rel_pos.y);
    }
  }

private:
  sprite m_sprite;

  static vec2 s_cam_pos;
};

