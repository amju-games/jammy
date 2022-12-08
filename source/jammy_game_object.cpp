#include "jammy_game_object.h"

vec2 jammy_game_object::s_cam_pos;

void jammy_game_object::set_cam_pos(const vec2& pos)
{
}

void jammy_game_object::draw(screen& dest)
{
  const vec2 CENTRE(60.f, 60.f);
  vec2 rel_pos = m_pos - s_cam_pos + CENTRE;

  int x = rel_pos.x;
  int y = rel_pos.y;

  m_sprite.draw(dest, x, y);
}

void jammy_game_object::update(float dt) 
{
  game_object::update(dt);
  m_sprite.update(dt);
}

