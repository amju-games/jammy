#include "global_palette.h"
#include "rock.h"

rock::rock()
{
  m_sprite.load("../assets/bird1.png", the_global_palette);
  m_sprite.set_num_cells(1, 1);

  m_pos = vec2(10, 10);
//  m_vel = vec2(2, 2);
}

