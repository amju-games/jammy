#include "directory.h"
#include "globals.h"
#include "hq.h"
#include "resources.h"

hq::hq()
{
  m_sprite.set_image(resources().get<image>(get_data_dir() + "player_HQ.png"));
  m_sprite.set_num_cells(1, 1);
  m_sprite.set_cell_range(0, 0);

  // HQ should move left-right
  m_pos = vec2(-500, 0);
  m_vel = vec2(5, 0); 
}

