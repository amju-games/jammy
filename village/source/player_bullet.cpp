#include "directory.h"
#include "globals.h"
#include "player_bullet.h"
#include "resources.h"

player_bullet::player_bullet() //const vec2& pos, const vec2& dir)
{
  m_sprite.set_image(resources().get<image>(get_data_dir() + "player_bullet.png"));
  m_sprite.set_num_cells(1, 1);
  m_sprite.set_cell_range(0, 0);
}

