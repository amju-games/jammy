#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <vector>

// Broad phase of SAP, produces list of potential colliding pairs
template<int NUM_AXES, class DERIVED_TYPE>
class sweep_and_prune 
{
public:
  using collision_vec = std::vector<std::array<DERIVED_TYPE*, 2>>;

  using id_vec = std::vector<std::pair<int, int>>;

  // For each axis, keep a sorted vec of min and max extents.
  // In the broad phase, we re-sort each axis vec, and swaps signal a potential collision.
  struct extent
  {
    int id = 0;
    float pos = 0; // extent (min or max) on this axis

    // Need this? So we only treat 2 of four cases as potential collisions??
    bool is_min = true; 
  };

  using extent_vec = std::vector<extent>;

  collision_vec broad_phase()
  {
    // TODO compare results from each axis. A pair should be included in the results if
    //  they are in the results for all axes.

    collision_vec ret;
    for (int axis = 0; axis < NUM_AXES; axis++)
    {
      id_vec result_for_this_axis = one_axis(axis);
    }
    return ret;
  }

  id_vec one_axis(int axis)
  {
    extent_vec& extents = m_extent_vecs[axis];
    return one_axis(extents);
  } 
 
  id_vec one_axis(extent_vec& extents)
  {
    id_vec ret;

    // Insertion sort
    const int size = static_cast<int>(extents.size());
    for (int i = 0; i < size; i++)
    {
      int j = i;
      while (j > 0 && extents[j - 1].pos > extents[j].pos)
      {
        std::swap(extents[j - 1], extents[j]);
        j--;
        // The objects referred to in the elements we just swapped are potential
        //  colliders
        ret.push_back(std::make_pair(extents[j - 1].pos, extents[j].pos));
      } 
    }

    return ret;
  }


  void add_extents(int axis, int obj_id, float min_value, float max_value)
  {
    add_extent(axis, extent{obj_id, min_value, true});
    add_extent(axis, extent{obj_id, max_value, false});
  }

  void add_extent(int axis, const extent& e)
  {
    assert(axis < NUM_AXES);
    add_extent(m_extent_vecs[axis], e);
  }

  void add_extent(extent_vec& ev, const extent& e)
  {
    ev.push_back(e);
  }

private:

  std::array<extent_vec, NUM_AXES> m_extent_vecs;

};


