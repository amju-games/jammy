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

  // Init game states
  the_attract_state = std::make_unique<attract_state>();
  the_enter_hi_score_state = std::make_unique<enter_hi_score_state>();
  the_game_over_state = std::make_unique<game_over_state>();
  the_load_level_state = std::make_unique<load_level_state>();
  the_new_game_state = std::make_unique<new_game_state>();
  the_play_state = std::make_unique<play_state>();
  the_show_hi_scores_state = std::make_unique<show_hi_scores_state>();
  the_splash_state = std::make_unique<splash_state>();

  the_sound_player = std::make_unique<sound_player_bass24>();

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

