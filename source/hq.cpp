#include "directory.h"
#include "globals.h"
#include "hq.h"

hq::hq()
{
  m_sprite.load(get_data_dir() + "player_HQ.png", the_global_palette);
  m_sprite.set_num_cells(1, 1);
  m_sprite.set_cell_range(0, 0);

  // HQ should move left-right
  m_pos = vec2(-500, 0);
  m_vel = vec2(5, 0); 
}

