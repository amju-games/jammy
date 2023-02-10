#pragma once

#ifdef NARROW_PHASE_DEBUG
#include <iostream>
#endif

#include <array>
#include <vector>
#include "jammy_game_object.h"

// Pixel-perfect narrow-phase collision check.
template<class DERIVED_TYPE>
class narrow_phase_pixel_perfect
{
public:
  using collision_vec = std::vector<std::pair<DERIVED_TYPE*, DERIVED_TYPE*>>;

  collision_vec narrow_phase(const collision_vec& poss_colliders) const
  {
    collision_vec ret;
    // ? worth doing? ret.reserve(poss_colliders.size());

    std::copy_if(
      poss_colliders.begin(), poss_colliders.end(),
      std::back_inserter(ret),
      [](const auto& p) 
      {
#ifdef NARROW_PHASE_DEBUG
        std::cout << "Narrow check: (" << p.first->print() << ") and (" << p.second->print() << ")\n";
#endif
        return sprite_collision(p.first, p.second); 
      });

    return ret;
  }
};

