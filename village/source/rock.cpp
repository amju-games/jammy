#include <cmath>
#include "config_file.h"
#include "deg_to_rad.h"
#include "directory.h"
#include "globals.h"
#include "image_32.h"
#include "resources.h"
#include "rng.h"
#include "rock.h"
#include "sound_player.h"
#include "universe.h"

namespace
{
  const float SPEED_LIMIT = 50.f; // TODO CONFIG

  vec2 gen_random_vel(float speed_limit)
  {
    return vec2(
      the_rng.rand(-speed_limit, speed_limit), 
      the_rng.rand(-speed_limit, speed_limit));

//    return vec2(rand() % speed_limit - speed_limit / 2, rand() % speed_limit - speed_limit / 2);
  }

  vec2 gen_random_pos(const vec2& centre)
  {
    // Pos should be just off screen.
    float angle = deg_to_rad(the_rng.rand(360.f)); ////static_cast<float>(rand() % 360));
    float dist = PRETEND_SCREEN_W;
    return centre + vec2(dist * cos(angle), dist * sin(angle));
  }

  int get_random_dir()
  {
    return (the_rng.rand(2) == 1) ? 1 : -1;
  }
}

rock::rock(int size_level, [[maybe_unused]] int child_index)
{
  auto config = resources().get<config_file>("config.txt");

  m_size_level = size_level;
  set_is_updatable(size_level == 0);
  set_is_collidable(size_level == 0);

  // TODO Variants.
  // TODO Use child_index so we don't have two children with the same sprite sheet.
  static const std::array<std::string, 3> filenames = 
  {
    "rotating_rock_0.png", "rotating_rock_1.png", "rotating_rock_2.png"
  };

  std::string filename = filenames[size_level];

  sprite s;
  s.set_image(resources().get<image>(get_data_dir() + filename));
 
  float cell_time_min = config->get_float("rock::cell_time_min");
  float cell_time_max = config->get_float("rock::cell_time_max");
  s.set_cell_time(the_rng.rand(cell_time_min, cell_time_max)); ////0.1f); // TODO some randomness?

  s.set_num_cells(4, 4);
  s.set_cell_range(0, 15); 
  s.set_cell_dir(get_random_dir());
  set_sprite(s);

  m_explosion.set_image(resources().get<image>(get_data_dir() + "explosion1_64.png"));

  // TODO this cell range should not loop
  m_explosion.set_num_cells(4, 4);
  m_explosion.set_cell_range(0, 15);
  m_explosion.set_cell_time(0.05f); // TODO CONFIG

  // Set random pos/vel
  m_pos = gen_random_pos(get_cam_pos());
  m_vel = gen_random_vel(SPEED_LIMIT);
}

int rock::get_score() const
{
  static const std::array<int, 3> SCORE = { 20, 50, 100 }; // TODO CONFIG

  return SCORE[m_size_level];
}

void rock::update(float dt)
{
  jammy_game_object::update(dt);

  if (m_is_exploding)
  {
    m_explosion.update(dt);
    if (m_explosion.get_cell() == 15)
    {
      set_is_updatable(false);
      m_is_exploding = false;
    }
  }
}

void rock::draw(ref_image dest)
{
  if (m_is_exploding)
  {
    draw_sprite<additive_blend>(m_explosion, dest);
  }
  else 
  {
    jammy_game_object::draw(dest);  
  }
}

void rock::explode()
{
  if (m_is_exploding)
  {
std::cout << "Rock already exploding!\n";
    return;
  }

  the_sound_player->play_wav(get_data_dir() + "sounds/Explosion2.wav");

  the_player_progress.add_score(get_score());

  the_level_manager.get_level().dec_num_rocks();

  //set_is_updatable(false); // wait until explosion finishes
  set_is_exploding(true);
  // Centre explosion sprite on position

  set_is_collidable(false);

  // Enable children
  for (auto& r : m_children)
  {
    r->set_is_updatable(true);
    r->set_is_collidable(true);

    r->set_pos(get_pos()); // plus an offset

    // Child rocks get combined vel
    r->set_vel(m_vel + gen_random_vel(SPEED_LIMIT));
  }
}

void rock::add_child(std::shared_ptr<rock>& child)
{
  m_children.push_back(child);
}


