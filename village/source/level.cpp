#include "circular_buffer.h"
#include "directory.h"
#include "globals.h"
#include "level.h"
#include "parallax_bg.h"
#include "sound_player.h"

void level::load()
{
  const int MAX_PLAYER_BULLETS = 10;
  m_player_bullets = std::make_unique<circular_buffer<player_bullet>>(the_game, MAX_PLAYER_BULLETS);

  m_player = nullptr;
  the_game.clear_game_objects();

  // Add background
  the_game.add_game_object(std::make_shared<parallax_bg>());

  // Add player
  m_player = std::make_shared<player>();
  the_game.add_game_object(m_player);

  // Add asteroids
  const int NUM_ROCKS = 1;

  for (int i = 0; i < NUM_ROCKS; i++)
  {
    // Rocks break up into child rocks. Let's do this recursively.
    add_rock_and_descendants(0, 0); // level 0: largest rock; index 0
  }
}

player& level::get_player()
{
  return *(m_player.get());
}

void level::dec_num_rocks()
{
  assert(m_num_rocks_in_level > 0);
  m_num_rocks_in_level--;

std::cout << "This many rocks left in level: " << m_num_rocks_in_level << "\n";

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

