//#include <cstdlib>

#include "directory.h"
#include "globals.h"
#include "image_32.h"
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
  std::string s = "rock_" + std::to_string(size) + ".png";

  m_sprite.set_image(resources().get<image>(get_data_dir() + s));
  m_sprite.set_num_cells(1, 1);
  m_sprite.set_cell_range(0, 0); 

  p_image expl = std::make_shared<image_32>();
  expl->load(get_data_dir() + "explosion1_64.png");
  m_explosion.set_image(expl); 
    //resources().get<image>(get_data_dir() + "explosion1_64.png"));
  m_explosion.set_num_cells(4, 4);
  m_explosion.set_cell_range(0, 15);
  m_explosion.set_cell_time(0.05f); // TODO CONFIG

  // Set random pos/vel
  m_pos = vec2(rand() % UNIVERSE_SIZE, rand() % UNIVERSE_SIZE);
  m_vel = vec2(rand() % SPEED_LIMIT - SPEED_LIMIT / 2, rand() % SPEED_LIMIT - SPEED_LIMIT / 2);
}

void rock::update(float dt)
{
  jammy_game_object::update(dt);

  if (m_is_exploding)
  {
    m_explosion.update(dt);
    if (m_explosion.get_cell() == 15)
    {
      set_is_alive(false);
      m_is_exploding = false;
    }
  }
}

void rock::draw(ref_image dest)
{
  jammy_game_object::draw(dest);  
  if (m_is_exploding)
  {
    draw_sprite(m_explosion, dest); // TODO blend additively
  }
}

void rock::explode()
{
  //set_is_alive(false); // wait until explosion finishes
  set_is_exploding(true);
  // Centre explosion sprite on position

  set_is_collidable(false);

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


