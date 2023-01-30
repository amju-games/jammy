#include "directory.h"
#include "globals.h"
#include "image_32.h"
#include "init_state.h"
#include "resources.h"
#include "sound_player_bass24.h"

void init_state::on_active()
{
  the_screen = std::make_shared<image_32>();
  the_screen->set_size(PRETEND_SCREEN_W, PRETEND_SCREEN_H);

// Init font
  the_font.set_image(resources().get<image>(get_data_dir() + "font1 - magenta.png"));
  the_font.set_num_cells(16, 4);

  // Init game states
  the_play_state.reset(new play_state);
  the_load_level_state.reset(new load_level_state);
  the_splash_state.reset(new splash_state);
  the_game_over_state.reset(new game_over_state);
  the_enter_hi_score_state.reset(new enter_hi_score_state);

  the_sound_player.reset(new sound_player_bass24);

  the_game.set_game_state(the_splash_state);
}


