#include "jammy_game_object.h"

vec2 jammy_game_object::s_cam_pos;

void jammy_game_object::set_cam_pos(const vec2& pos)
{
}

void jammy_game_object::draw(screen& dest)
{
  int x = (m_pos + s_cam_pos).x;
  int y = (m_pos + s_cam_pos).y;

  m_sprite.draw(dest, x, y);
}

void jammy_game_object::update(float dt) 
{
  game_object::update(dt);
  m_sprite.update(dt);
}

