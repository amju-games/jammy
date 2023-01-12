#include "walker.h"

const float WALK_FRAME_TIME = .2f;

void walker::update(float dt)
{
  // Using robotron sprites: a strip of 12, 3 for each dir: left, right, down, up.
  const int FORWARD = 1;
  m_sprite.set_cell_time(WALK_FRAME_TIME);
  m_sprite.set_bounce_mode(true); // go back and forward thru cells in range

  const float EPSILON = 1.f;
  if (m_vel.x < -EPSILON)
  {
    m_sprite.set_cell_range(0, 2);
  }
  else if (m_vel.x > EPSILON)
  {
    m_sprite.set_cell_range(3, 5);
  }
  else if (m_vel.y < -EPSILON)
  {
    m_sprite.set_cell_range(9, 11);
  }
  else if (m_vel.y > EPSILON)
  {
    m_sprite.set_cell_range(6, 8);
  }
  else 
  {
    m_sprite.set_cell_time(0);
  }

  jammy_game_object::update(dt);
}

