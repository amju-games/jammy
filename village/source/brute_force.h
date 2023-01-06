#pragma once

#include <array>
#include <cassert>
#include <vector>
#include <game.h>
#include "jammy_game_object.h"

// Brute force broad phase collision detection
// Simply make all possible pairs of game objects. We only need to create the
//  collection when the game objects change.
// This doesn't scale well (n^2) but is a good comparision with other algorithms,
//  i.e. sweep and prune.
template <class DERIVED_TYPE>
class brute_force
{
public:
  using collision_vec = std::vector<std::array<DERIVED_TYPE*, 2>>;

  void set_game_objects(const game_objects* gos)  
  {
    m_game_objects = gos;
  }

  collision_vec broad_phase()
  {
    assert(m_game_objects);
 
    collision_vec ret;
    const int num_game_objects = m_game_objects->size(); 
    for (int i = 0; i < num_game_objects; i++)
    {
      for (int j = i + 1; j < num_game_objects; j++)
      {
        ret.push_back(std::array<DERIVED_TYPE*, 2>{
          dynamic_cast<DERIVED_TYPE*>((*m_game_objects)[i].get()),
          dynamic_cast<DERIVED_TYPE*>((*m_game_objects)[j].get()) 
        });
      }
    }
    return ret;
  };

private:
  const game_objects* m_game_objects = nullptr;
};

