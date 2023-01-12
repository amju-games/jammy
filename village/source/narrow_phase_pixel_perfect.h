#pragma once

#include <array>
#include <vector>
#include "jammy_game_object.h"

// Pixel-perfect narrow-phase collision check.
template<class DERIVED_TYPE>
class narrow_phase_pixel_perfect
{
public:
  using collision_vec = std::vector<std::array<DERIVED_TYPE*, 2>>;

  collision_vec narrow_phase(const collision_vec& poss_colliders) const
  {
    collision_vec ret;
    // ? worth doing? ret.reserve(poss_colliders.size());

    std::copy_if(
      poss_colliders.begin(), poss_colliders.end(),
      std::back_inserter(ret),
      [](const auto& p) { return sprite_collision(p[0], p[1]); });

    return ret;
  }
};
