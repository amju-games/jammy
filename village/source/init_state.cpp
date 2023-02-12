#include "directory.h"
#include "globals.h"
#include "image_32.h"
#include "init_state.h"
#include "level_manager.h"
#include "resources.h"
#include "sound_player_bass24.h"

void init_state::on_active()
{
  the_screen = std::make_shared<image_32>();
  the_screen->set_size(PRETEND_SCREEN_W, PRETEND_SCREEN_H);

// Init font
  the_font.set_image(resources().get<image>(get_data_dir() + "font_5x5.png"));
  the_font.set_num_cells(16, 4);

  // Init game states
  the_enter_hi_score_state.reset(new enter_hi_score_state);
  the_game_over_state.reset(new game_over_state);
  the_load_level_state.reset(new load_level_state);
  the_new_game_state.reset(new new_game_state);
  the_play_state.reset(new play_state);
  the_show_hi_scores_state.reset(new show_hi_scores_state);
  the_splash_state.reset(new splash_state);

  the_sound_player.reset(new sound_player_bass24);

  load_attract_level();

  the_game.set_game_state(the_show_hi_scores_state); //the_splash_state);

  the_hi_scores.load(hi_scores_filename());
}

void init_state::load_attract_level()
{
  the_level_manager.make_new_level();
  level& lev = the_level_manager.get_level();
  lev.set_level_num(0);
  lev.load();
}

