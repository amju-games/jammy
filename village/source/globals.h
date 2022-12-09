#pragma once

#include <memory>
#include "enter_hi_score_state.h"
#include "font.h"
#include "game.h"
#include "game_over_state.h"
#include "hi_scores.h"
#include "human_list.h"
#include "palette.h"
#include "play_state.h"
#include "screen.h"
#include "sound_player.h"
#include "splash_state.h"
#include "timer.h"

extern std::unique_ptr<sound_player> the_sound_player;
extern font the_font;
extern game the_game;
extern std::unique_ptr<game_over_state> the_game_over_state;
extern std::unique_ptr<enter_hi_score_state> the_enter_hi_score_state;
extern hi_scores the_hi_score_table;
extern human_list the_human_list;
extern palette the_global_palette;
extern std::unique_ptr<play_state> the_play_state;
extern screen the_screen;
extern std::unique_ptr<splash_state> the_splash_state;
extern timer the_timer;

