#include "circular_buffer.h"
#include "directory.h"
#include "globals.h"
#include "level.h"
#include "parallax_bg.h"
#include "sound_player.h"

bool level::is_level_completed() const
{
  return m_num_rocks_in_level == 0;
}

void level::col_det()
{
  m_collision_mgr.check_for_collisions();
}

void level::load()
{
  m_player = nullptr;
  the_game.clear_game_objects();

  // Add background: do first, so drawn first, until we have some kind of z-ordering
  the_game.add_game_object(std::make_shared<parallax_bg>());

  populate_collision_funcs(m_collision_mgr);

  const int MAX_PLAYER_BULLETS = 10;
  m_player_bullets = std::make_unique<circular_buffer<player_bullet>>(the_game, MAX_PLAYER_BULLETS);
  m_player_bullets->pre_populate_buffer();

  // Add player
  m_player = std::make_shared<player>();
  the_game.add_game_object(m_player);
  jammy_game_object::set_cam_pos(m_player->get_pos());

  // Add asteroids
  // TODO good formula for this
  const int MAX_NUM_ROCKS = 1000; //15;
  int num_rocks = std::min(MAX_NUM_ROCKS, m_level_num * 100);

  m_num_rocks_in_level = 0;
  for (int i = 0; i < num_rocks; i++)
  {
    // Rocks break up into child rocks. Let's do this recursively.
    add_rock_and_descendants(0, 0); // level 0: largest rock; index 0
  }

#ifdef ROCK_COUNT_DEBUG
std::cout << "This many rocks in level: " << m_num_rocks_in_level << "\n";
#endif

  // At end of load. We copy all game objects? Or copy ptr to container?
  m_collision_mgr.set_game_objects(the_game.get_game_objects());
}

player& level::get_player()
{
  return *(m_player.get());
}

void level::dec_num_rocks()
{
  assert(m_num_rocks_in_level > 0);
  m_num_rocks_in_level--;

#ifdef ROCK_COUNT_DEBUG
std::cout << "This many rocks left in level: " << m_num_rocks_in_level << "\n";
#endif

  if (m_num_rocks_in_level == 0)
  {
    // TODO New level
  }
}

std::shared_ptr<rock> level::add_rock_and_descendants(int level, int child_index)
{
  auto r = std::make_shared<rock>(level, child_index);
  the_game.add_game_object(r);
  m_num_rocks_in_level++;

  if (level < 2)
  {
    for (int i = 0; i < 2; i++) // TODO a bit of variety in num children
    {
      // Pass in i so each child can have a different sprite sheet
      auto child = add_rock_and_descendants(level + 1, i);
      r->add_child(child);
    }
  }

  return r;
}

void level::add_player_bullet()
{
  // Use a circular buffer for player bullets.
  // If we haven't filled up the buffer, create a new player_bullet, add it to the game and
  //  the buffer. If we have filled the buffer, reset the player_bullet we are currently pointing
  //  to, and inc to the next slot.

  auto pb = m_player_bullets->get_next_element();
  pb->fire(m_player);

  // TODO Appropriate sound FX
  the_sound_player->play_wav(get_data_dir() + "sounds/sfx_sounds_impact3.wav");
}

