#include <iostream>
#include "directory.h"
#include "globals.h"
#include "parallax_bg.h"
#include "screen.h"

parallax_bg::parallax_bg()
{
  const std::string FILENAMES[] = 
  {
    "Background_tiled.png",
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

    int w = im.get_width();
    int h = im.get_height();
    int x_edge = w - screen::WIDTH;
    int y_edge = h - screen::HEIGHT;

    // Wrap x and y
    if (pos.x > 0)
    {
      pos.x = -x_edge;
    }
    else if (pos.x < -x_edge)
    {
      pos.x = 0; 
    }

    if (pos.y > 0)
    {
      pos.y = -y_edge;
    }
    else if (pos.y < -y_edge)
    {
      pos.y = 0; 
    }

    im.blit(dest, pos.x, pos.y); 
  }
}

