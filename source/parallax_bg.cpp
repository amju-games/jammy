#include <iostream>
#include "directory.h"
#include "global_palette.h"
#include "parallax_bg.h"
#include "screen.h"

parallax_bg::parallax_bg()
{
  const std::string FILENAMES[] = 
  {
    "Background.png",
  };

  for (int i = 0; i < NUM_IMAGES; i++)
  {
    m_images[i].load(get_data_dir() + FILENAMES[i], the_global_palette);
  }
}

void parallax_bg::update(float dt)
{
  static vec2 old_pos = s_cam_pos;
  vec2 cam_change = old_pos - s_cam_pos;
  old_pos = s_cam_pos;

  const float SPEED[] = { 1.f, 0.1f, 0.2f }; // etc

  for (int i = 0; i < NUM_IMAGES; i++)
  {
    m_bg_pos[i] += cam_change * SPEED[i];
  }
}

void parallax_bg::draw(screen& dest)
{
  for (int i = 0; i < NUM_IMAGES; i++)
  {
    const image& im = m_images[i];
    vec2& pos = m_bg_pos[i];

    //vec2 rel_pos = m_pos - s_cam_pos * SPEED[i];
 
    //int x = rel_pos.x;
    //int y = rel_pos.y;

    int w = im.get_width();
    int h = im.get_height();
    int x_edge = w - screen::WIDTH;
    int y_edge = h - screen::HEIGHT;

    // Wrap x and y
    if (pos.x > 0)
    {
      pos.x = -w; //x_edge;
    }
    else if (pos.x < -w) //x_edge)
    {
      pos.x = 0; 
    }
/*
    if (y < 0)
    {
      y = y_edge;
    }
    else if (y > y_edge)
    {
      y = 0;
    } 
*/

std::cout << "BG x: " << pos.x << " y: " << pos.y << "\n";

    im.blit(dest, pos.x, pos.y); 
    // Blit again for wrapping??
  }
}

