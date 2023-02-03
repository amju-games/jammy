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
template <class DERIVED_TYPE, class DOUBLE_DISPATCHER>
class brute_force
{
public:
  using collision_vec = std::vector<std::array<DERIVED_TYPE*, 2>>;

  void set_game_objects(const game_objects* gos)  
  {
    m_game_objects = gos;
  }

  collision_vec broad_phase(const DOUBLE_DISPATCHER& dd)
  {
    assert(m_game_objects);
 
    collision_vec ret;
    const int num_game_objects = m_game_objects->size(); 
    for (int i = 0; i < num_game_objects; i++)
    {
      if ((*m_game_objects)[i]->is_collidable() == false)
      {
        continue;
      }

      for (int j = i + 1; j < num_game_objects; j++)
      {
        if ((*m_game_objects)[j]->is_collidable())
        {
          DERIVED_TYPE* obj1 = dynamic_cast<DERIVED_TYPE*>((*m_game_objects)[i].get());
          DERIVED_TYPE* obj2 = dynamic_cast<DERIVED_TYPE*>((*m_game_objects)[j].get());

          if (dd.has_handler(obj1, obj2))
          {
            ret.push_back(std::array<DERIVED_TYPE*, 2>{ obj1, obj2 });
          }
        }
      }
    }

    return ret;
  };

private:
  const game_objects* m_game_objects = nullptr;
};

