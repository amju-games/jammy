#include "globals.h"

level_manager the_level_manager;
p_image the_screen;
std::unique_ptr<sound_player> the_sound_player;
game the_game;
std::unique_ptr<game_over_state> the_game_over_state;
std::unique_ptr<enter_hi_score_state> the_enter_hi_score_state;
hi_scores the_hi_score_table;
font the_font;
std::unique_ptr<load_level_state> the_load_level_state;
std::unique_ptr<play_state> the_play_state;
std::unique_ptr<splash_state> the_splash_state;
timer the_timer;

