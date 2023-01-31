#pragma once

#include <memory>
#include "enter_hi_score_state.h"
#include "font.h"
#include "game.h"
#include "game_over_state.h"
#include "hi_scores.h"
#include "level_manager.h"
#include "load_level_state.h"
#include "play_state.h"
#include "sound_player.h"
#include "splash_state.h"
#include "timer.h"

extern p_image the_screen;

extern font the_font; // TODO Should not be a global. Font is a resource
extern game the_game;
extern timer the_timer; // can't this be owned by the game?

extern std::unique_ptr<sound_player> the_sound_player; // owned by game?

extern level_manager the_level_manager;

// Hmm
extern hi_scores the_hi_score_table;

// Game states. Should these be stored, e.g. in game?
extern std::unique_ptr<game_over_state> the_game_over_state;
extern std::unique_ptr<enter_hi_score_state> the_enter_hi_score_state;
extern std::unique_ptr<load_level_state> the_load_level_state;
extern std::unique_ptr<play_state> the_play_state;
extern std::unique_ptr<splash_state> the_splash_state;

