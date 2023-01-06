#include <iostream>
#include "directory.h"
#include "globals.h"
#include "parallax_bg.h"
#include "screen.h"
#include "universe.h"

parallax_bg::parallax_bg()
{
  const std::string FILENAMES[] = 
  {
    "star_bg.png", // TODO use as bg star for now
  };

  for (int i = 0; i < NUM_IMAGES; i++)
  {
    auto& s = m_sprites[i];

    s.load(get_data_dir() + FILENAMES[i], the_global_palette);
    s.set_cell_time(.4f); // TODO TEMP TEST
    s.set_num_cells(4, 1); 
    s.set_cell_range(0, 3);
  }
}

void parallax_bg::update(float dt)
{
  static vec2 old_pos = s_cam_pos;
  vec2 cam_change = old_pos - s_cam_pos;
  old_pos = s_cam_pos;

  const float SPEED[] = { 1.f, 0.75f, 0.25f }; // etc

  for (int i = 0; i < NUM_LAYERS; i++)
  {
    vec2& p = m_bg_pos[i];

    p += cam_change * SPEED[i];

    if (p.x > UNIVERSE_SIZE / 2) { p.x -= UNIVERSE_SIZE; }
    if (p.x < -UNIVERSE_SIZE / 2) { p.x += UNIVERSE_SIZE; }
    if (p.y > UNIVERSE_SIZE / 2) { p.y -= UNIVERSE_SIZE; }
    if (p.y < -UNIVERSE_SIZE / 2) { p.y += UNIVERSE_SIZE; }
  }

  // Update sprite cells
  for (int i = 0; i < NUM_IMAGES; i++)
  {
    auto& s = m_sprites[i];
    s.update(dt);
  }
}

vec2 get_random_star_pos(int layer, int star)
{
  int p = (layer + 1) * 17 + star * 13; // TODO TEMP TEST
  p *= p;

  int q = (layer + 1) * 31 + star * 37;
  q *= q;
 
  vec2 pos(static_cast<float>(p % UNIVERSE_SIZE), static_cast<float>(q % UNIVERSE_SIZE));
  return pos;
}

void parallax_bg::draw(screen& dest)
{
  // Clear screen to colour index 1, which we know is black because we added it in main 
  //  explicitly.
  dest.clear(1); // TODO get this index from somewhere

  // Draw most distant layer first
  for (int i = NUM_LAYERS - 1; i >= 0; i--)
  {
    // TODO An array of images for each layer?
    const auto& s = m_sprites[0];

    const int NUM_STARS_PER_LAYER = 100;
    for (int j = 0; j < NUM_STARS_PER_LAYER; j++)
    {
      const vec2 star_pos = get_random_star_pos(i, j);
      const vec2 p = m_bg_pos[i] + star_pos + CENTRE_SCREEN;
      s.draw(dest, p.x, p.y); 
      s.draw(dest, p.x - UNIVERSE_SIZE, p.y); 
      s.draw(dest, p.x, p.y - UNIVERSE_SIZE); 
      s.draw(dest, p.x - UNIVERSE_SIZE, p.y - UNIVERSE_SIZE); 
    }
  }
}

