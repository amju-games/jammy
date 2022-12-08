#include <iostream>
#include <cassert>
#include "game.h"
#include "jammy_game_object.h"
#include "play_state.h"
#include "screen.h"

void play_state::update(float dt)
{
  assert(m_game);
  m_game->update_game_objects(dt); 
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

