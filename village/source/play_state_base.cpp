#include <iostream>
#include <bitset>

#include <cassert>
#include "game.h"
#include "globals.h"
#include "jammy_game_object.h"

void play_state_base::on_active()
{
  jammy_game_state::on_active();
  m_hud.load();
}
 
void play_state_base::draw() 
{
  draw_game_objects();

  draw_hud();
  
  // Explicitly call this from subclass draw() 
  //jammy_game_state::draw(); // fps
}

void play_state_base::update(float dt)
{
  jammy_game_state::update(dt);

  the_game.update_game_objects(dt); 

  the_level_manager.get_level().col_det();

  m_hud.update(dt);
}

void play_state_base::draw_game_objects()
{
  game_objects* gos = the_game.get_game_objects();
  for (p_game_object& go : *gos)
  {
    jammy_game_object* jgo = dynamic_cast<jammy_game_object*>(go.get());
    assert(jgo);
    jgo->draw(the_screen);
  }
}
 
void play_state_base::draw_hud()
{
  m_hud.draw();
}

