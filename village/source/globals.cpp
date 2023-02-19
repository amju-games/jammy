#include "globals.h"

player_progress the_player_progress;
level_manager the_level_manager;
std::unique_ptr<sound_player> the_sound_player;
game the_game;
hi_scores the_hi_scores;
rng the_rng;
p_image the_screen;
timer the_timer;

std::unique_ptr<attract_state> the_attract_state;
std::unique_ptr<enter_hi_score_state> the_enter_hi_score_state;
std::unique_ptr<game_over_state> the_game_over_state;
std::unique_ptr<load_level_state> the_load_level_state;
std::unique_ptr<new_game_state> the_new_game_state;
std::unique_ptr<play_state> the_play_state;
std::unique_ptr<show_hi_scores_state> the_show_hi_scores_state;
std::unique_ptr<splash_state> the_splash_state;

