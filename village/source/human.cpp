#include "human.h"
#include "directory.h"
#include "globals.h"
#include "player.h"
#include "resources.h"
#include "universe.h"

void plot(int x, int y, ref_image dest)
{
  const colour ROPE_COLOUR(0xff, 0xff, 0);
  if (x >= 0 && x < PRETEND_SCREEN_W && y >= 0 && y < PRETEND_SCREEN_H)
  {
    dest->set_colour(x, y, ROPE_COLOUR);
  }
}

// https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C.2B.2B
void plot_line(int x1, int y1, int x2, int y2, ref_image dest)
{
  // Bresenham's line algorithm
  const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
  if(steep)
  {
    std::swap(x1, y1);
    std::swap(x2, y2);
  }
 
  if(x1 > x2)
  {
    std::swap(x1, x2);
    std::swap(y1, y2);
  }
 
  const float dx = x2 - x1;
  const float dy = fabs(y2 - y1);
 
  float error = dx / 2.0f;
  const int ystep = (y1 < y2) ? 1 : -1;
  int y = (int)y1;
 
  const int maxX = (int)x2;
 
  for(int x=(int)x1; x<maxX; x++)
  {
    if(steep)
    {
        plot(y, x, dest);
    }
    else
    {
        plot(x, y, dest);
    }
 
    error -= dy;
    if(error < 0)
    {
        y += ystep;
        error += dx;
    }
  }
}

human::human()
{
  m_sprite.set_image(resources().get<image>(get_data_dir() + "Human_all.png"));
  m_sprite.set_num_cells(9, 1);
  m_sprite.set_cell_range(0, 0);

  const int SPEED_LIMIT = 50; // TODO
  // Set random pos/vel
  m_pos = vec2(rand() % UNIVERSE_SIZE, rand() % UNIVERSE_SIZE);
  m_vel = vec2(rand() % SPEED_LIMIT - SPEED_LIMIT / 2, rand() % SPEED_LIMIT - SPEED_LIMIT / 2);
}

void human::update(float dt)
{
  jammy_game_object::update(dt);

  if (m_rescued)
  {
    // move towards 'player' - or human at end of chain
    vec2 vel = m_parent->get_pos() - get_pos();
    // Max speed? TODO 
    set_vel(vel * 0.9f); // TODO TEMP TEST
  }
}

void human::draw(ref_image dest)
{
  // Draw rope between this human and parent
  if (m_parent)
  {
    const vec2 OFFSET(5, 5);
    vec2 v1 = m_pos - s_cam_pos + OFFSET + CENTRE_SCREEN; 
    vec2 v2 = m_parent->get_pos() - s_cam_pos + OFFSET + CENTRE_SCREEN; 

    plot_line(v1.x, v1.y, v2.x, v2.y, dest);
  }

  jammy_game_object::draw(dest);
}

