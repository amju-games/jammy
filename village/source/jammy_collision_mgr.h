#pragma once

#include "brute_force.h"
#include "collision_mgr.h"
#include "double_dispatcher.h"
#include "narrow_phase_pixel_perfect.h"

using jammy_collision_mgr = 
  collision_mgr<
    double_dispatcher<jammy_game_object>,
    brute_force<jammy_game_object>,
    narrow_phase_pixel_perfect<jammy_game_object>
  >;

void populate_collision_funcs(jammy_collision_mgr&);

