#include "global_palette.h"
#include "player.h"

player::player()
{
  m_sprite.load("../assets/Player_center_down.png", the_global_palette);
  m_sprite.set_num_cells(1, 1);

  m_pos = vec2(60, 60);
}

void player::move(int move_dir)
{
  float speed = 10.f; // TODO 

  vec2 dir;
  if (move_dir & MOVE_UP)
  {
    dir.y -= speed; 
  }
  if (move_dir & MOVE_DOWN)
  {
    dir.y += speed; 
  }
  if (move_dir & MOVE_LEFT)
  {
    dir.x -= speed; 
  }
  if (move_dir & MOVE_RIGHT)
  {
    dir.x += speed; 
  }

  m_vel += dir;
  // Max speed
  float max_speed = 20.f; // TODO
  if (squared_length(m_vel) > max_speed)
  {
    m_vel = normalise(m_vel) * max_speed;
  }
}

