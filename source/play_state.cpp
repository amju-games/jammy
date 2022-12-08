#include <iostream>
#include <cassert>
#include "game.h"
#include "jammy_game_object.h"
#include "parallax_bg.h"
#include "player.h"
#include "play_state.h"
#include "rock.h"
#include "screen.h"

void play_state::on_input(int input)
{
  assert(m_player);
  m_player->move(input);  
}

void play_state::on_active() 
{
  std::cout << "ON ACTIVE\n";

  assert(m_game);
  m_game->add_game_object(new parallax_bg);

  m_player = new player;
  m_game->add_game_object(m_player);

  int NUM_ROCKS = 50; 
  for (int i = 0; i < NUM_ROCKS; i++)
  {
    m_game->add_game_object(new rock);
  }
}

void play_state::on_deactive() 
{
  m_player = nullptr; // TODO leak!
}

void play_state::col_det()
{
  // Test player against rocks
  assert(m_game);
  game_objects* gos = m_game->get_game_objects();
  for (game_object* go : *gos)
  {
    rock* r = dynamic_cast<rock*>(go);
    if (r)
    {
      // Check player/rock sprites
      if (sprite_collision(r, m_player))
      {
        std::cout << "ARGHH!\n";
      }
    }
  }
}

void play_state::update(float dt)
{
  assert(m_game);
  m_game->update_game_objects(dt); 

  col_det();
}

void play_state::draw() 
{
  assert(m_game);
  assert(m_screen);
  game_objects* gos = m_game->get_game_objects();
  for (game_object* go : *gos)
  {
    jammy_game_object* jgo = dynamic_cast<jammy_game_object*>(go);
    assert(jgo);
    jgo->draw(*m_screen);
  }
}

