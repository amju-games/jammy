#include "moon_state.h"
#include "globals.h"
#include "moon.h"

void moon_state::on_active()
{
  play_state_base::on_active();

  the_game.clear_game_objects();
  auto m = std::make_shared<moon>();
  m->set_pos(vec2(-128, -64));
  the_game.add_game_object(m);
}

