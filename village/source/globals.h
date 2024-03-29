#pragma once

#include <memory>
#include "attract_state.h"
#include "enter_hi_score_state.h"
#include "game.h"
#include "game_over_state.h"
#include "hi_scores.h"
#include "level_manager.h"
#include "load_level_state.h"
#include "moon_state.h"
#include "new_game_state.h"
#include "player_progress.h"
#include "play_state.h"
#include "rng.h"
#include "show_hi_scores_state.h"
#include "sound_player.h"
#include "splash_state.h"
#include "timer.h"

extern p_image the_screen;

extern game the_game;
extern timer the_timer; // can't this be owned by the game?

extern std::unique_ptr<sound_player> the_sound_player; // owned by game?

extern level_manager the_level_manager;

extern player_progress the_player_progress;

extern rng the_rng;

// Hmm
extern hi_scores the_hi_scores;

// Game states. Should these be stored, e.g. in game?
extern std::unique_ptr<attract_state> the_attract_state;
extern std::unique_ptr<game_over_state> the_game_over_state;
extern std::unique_ptr<enter_hi_score_state> the_enter_hi_score_state;
extern std::unique_ptr<load_level_state> the_load_level_state;
extern std::unique_ptr<moon_state> the_moon_state;
extern std::unique_ptr<new_game_state> the_new_game_state;
extern std::unique_ptr<play_state> the_play_state;
extern std::unique_ptr<show_hi_scores_state> the_show_hi_scores_state;
extern std::unique_ptr<splash_state> the_splash_state;

