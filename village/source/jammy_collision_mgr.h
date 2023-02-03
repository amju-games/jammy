#pragma once

#include "brute_force.h"
#include "collision_mgr.h"
#include "double_dispatcher.h"
#include "narrow_phase_pixel_perfect.h"

using dd = double_dispatcher<jammy_game_object>;

using jammy_collision_mgr = 
  collision_mgr<
    dd,
    brute_force<jammy_game_object, dd>,
    narrow_phase_pixel_perfect<jammy_game_object>
  >;

void populate_collision_funcs(jammy_collision_mgr&);

