#pragma once

#include "game_object.h"
#include "screen.h"
#include "sprite.h"

class jammy_game_object : public game_object
{
public:
  // load

  void set_cam_pos(const vec2& pos);
  virtual void draw(screen& scr);
  void update(float dt) override; 

protected:
  sprite m_sprite;
  bool m_is_alive = true; // ?

  static vec2 s_cam_pos;
};

