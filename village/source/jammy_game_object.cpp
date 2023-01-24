#include "jammy_game_object.h"
#include "universe.h"

vec2 jammy_game_object::s_cam_pos;

bool sprite_collision(
  jammy_game_object* jgo1, jammy_game_object* jgo2)
{
  pix_int_result r = pixel_intersect(
    jgo1->m_sprite, jgo1->m_sprite.get_cell(), jgo1->m_pos.x, jgo1->m_pos.y,
    jgo2->m_sprite, jgo2->m_sprite.get_cell(), jgo2->m_pos.x, jgo2->m_pos.y);

  return (r == pix_int_result::YES_COLLIDE);
}

void jammy_game_object::set_cam_pos(const vec2& pos)
{
  s_cam_pos = pos;
}

void jammy_game_object::draw(ref_image dest)
{
  if (!is_alive())
  {
    return;
  }

  draw_sprite(m_sprite, dest);
}

void jammy_game_object::draw_sprite(const sprite& s, ref_image dest) const
{
  vec2 rel_pos = m_pos - s_cam_pos + CENTRE_SCREEN;

  int x = rel_pos.x;
  int y = rel_pos.y;

  s.draw(dest, x, y);
}

void jammy_game_object::update(float dt) 
{
  if (!is_alive())
  {
    return;
  }

  game_object::update(dt);

  // Wrap around
  vec2 rel_pos = m_pos - s_cam_pos;

  if (rel_pos.x < -(UNIVERSE_SIZE / 2 + m_sprite.get_cell_w()))
  {
    m_pos.x += UNIVERSE_SIZE + m_sprite.get_cell_w();
  }
  if (rel_pos.x > (UNIVERSE_SIZE / 2))
  {
    m_pos.x -= (UNIVERSE_SIZE + m_sprite.get_cell_w());
  }
  if (rel_pos.y < -(UNIVERSE_SIZE / 2 + m_sprite.get_cell_h()))
  {
    m_pos.y += UNIVERSE_SIZE + m_sprite.get_cell_h();
  }
  if (rel_pos.y > (UNIVERSE_SIZE / 2))
  {
    m_pos.y -= (UNIVERSE_SIZE + m_sprite.get_cell_h());
  }

  m_sprite.update(dt);
}

