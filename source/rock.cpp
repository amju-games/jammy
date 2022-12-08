#include <cstdlib>
#include "directory.h"
#include "global_palette.h"
#include "rock.h"
#include "universe.h"

rock::rock()
{
  const int NUM_ASTEROID_TYPES = 11;
  int r = rand() % NUM_ASTEROID_TYPES + 1;
  std::string s = "asteroid_" + std::to_string(r) + ".png";

  m_sprite.load(get_data_dir() + s, the_global_palette);
  m_sprite.set_num_cells(1, 1);
  m_sprite.set_cell_range(0, 0); 

  const int SPEED_LIMIT = 50; // TODO

  // Set random pos/vel
  m_pos = vec2(rand() % UNIVERSE_SIZE, rand() % UNIVERSE_SIZE);
  m_vel = vec2(rand() % SPEED_LIMIT - SPEED_LIMIT / 2, rand() % SPEED_LIMIT - SPEED_LIMIT / 2);
}


