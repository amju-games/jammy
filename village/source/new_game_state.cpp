#include "globals.h"
#include "load_level_state.h"
#include "new_game_state.h"

void new_game_state::on_active()
{
  jammy_game_state::on_active();
  the_player_progress.initialise();
}

void new_game_state::update(float dt)
{
  jammy_game_state::update(dt);
  the_game.set_game_state(the_load_level_state);
}

