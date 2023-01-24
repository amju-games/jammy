#include <cstdlib>
#include "directory.h"
#include "globals.h"
#include "resources.h"
#include "rock.h"
#include "universe.h"

namespace
{
  const int SPEED_LIMIT = 50; // TODO
}

rock::rock(int size)
{
  set_is_alive(size == 0);
  set_is_collidable(size == 0);

  // TODO Variants
  std::string s = "asteroid_" + std::to_string(size + 1) + ".png";

  m_sprite.set_image(resources().get<image>(get_data_dir() + s));
  m_sprite.set_num_cells(1, 1);
  m_sprite.set_cell_range(0, 0); 

  // Set random pos/vel
  m_pos = vec2(rand() % UNIVERSE_SIZE, rand() % UNIVERSE_SIZE);
  m_vel = vec2(rand() % SPEED_LIMIT - SPEED_LIMIT / 2, rand() % SPEED_LIMIT - SPEED_LIMIT / 2);
}

void rock::explode()
{
  // Enable children
  for (auto& r : m_children)
  {
    r->set_is_alive(true);
    r->set_is_collidable(true);

    r->set_pos(get_pos()); // plus an offset
    r->set_vel(vec2(rand() % SPEED_LIMIT - SPEED_LIMIT / 2, rand() % SPEED_LIMIT - SPEED_LIMIT / 2)); // maybe smaller faster?
  }
}

void rock::add_child(std::shared_ptr<rock>& child)
{
  m_children.push_back(child);
}


