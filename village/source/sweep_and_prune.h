#pragma once

#define SAP_DEBUG

#ifdef SAP_DEBUG
#include <iostream>
#endif

#include <algorithm>
#include <array>
#include <cassert>
#include <vector>
#include "collision_mgr.h"
#include "game.h"

// Broad phase of SAP, produces list of potential colliding pairs
template<int NUM_AXES, class DERIVED_TYPE, class DOUBLE_DISPATCHER>
class sweep_and_prune 
{
public:
  using coll_pair = std::pair<DERIVED_TYPE*, DERIVED_TYPE*>;
  coll_pair make_coll_pair(DERIVED_TYPE* obj1, DERIVED_TYPE* obj2)
  {
    assert(obj1->get_id() != obj2->get_id()); // can't collide with self
    coll_pair cp(obj1, obj2);
    // Always order the two objects the same way 
    if (cp.first->get_id() > cp.second->get_id())
    {
      std::swap(cp.first, cp.second);
    }
    return cp;
  }

  // For sort/uniq:
  static bool eq(const coll_pair& p1, const coll_pair& p2)
  {
    assert(p1.first->get_id() < p1.second->get_id());
    assert(p2.first->get_id() < p2.second->get_id());
    return p1.first == p2.first && p1.second == p2.second;
  }

  static bool lt(const coll_pair& p1, const coll_pair& p2)
  {
    assert(p1.first->get_id() < p1.second->get_id());
    assert(p2.first->get_id() < p2.second->get_id());
    if (p1.first->get_id() < p2.first->get_id()) return true;
    if (p1.first->get_id() > p2.first->get_id()) return false;
    return p1.second->get_id() < p2.second->get_id();
  }

  using collision_vec = std::vector<coll_pair>;

  void set_game_objects(const game_objects* gos)
  {
    m_game_objects = gos;
    populate_extents(*gos);
#ifdef SAP_DEBUG
std::cout << "SAP: num extents: " << m_extent_vecs[0].size() << "\n";
#endif
  }

  collision_vec broad_phase(const DOUBLE_DISPATCHER& dd)
  {
    assert(m_game_objects);

    update_extents();

    collision_vec ret;
    for (int axis = 0; axis < NUM_AXES; axis++)
    {
      one_axis(axis, ret, dd);
    }

    // Remove duplicates: sort, unique, erase
    std::sort(ret.begin(), ret.end(), 
              [](const coll_pair& cp1, const coll_pair& cp2) 
              { 
                return lt(cp1, cp2);
              }
    );
    ret.erase(std::unique(ret.begin(), ret.end(), 
                          [](const coll_pair& cp1, const coll_pair& cp2) 
                          { 
                            return lt(cp1, cp2);
                          } ),
              ret.end());

    // Check for intersecting AABBs

#ifdef SAP_DEBUG
std::cout << "SAP: result has " << ret.size() << " pairs.\n";
#endif
    return ret;
  }

//private:

  // Update pos for each extent
  void update_extents()
  {
    for (int axis = 0; axis < NUM_AXES; axis++)
    {
      update_extents_for_one_axis(axis);
    }
  }

  void update_extents_for_one_axis(int axis)
  {
    extent_vec& ev = get_extent_vec(axis);
    for (extent& e : ev)
    {
      assert(e.go);
      e.pos = e.is_min ? get_min_axis_value(*e.go, axis) : get_max_axis_value(*e.go, axis);
    }
  }

  // For each axis, keep a sorted vec of min and max extents.
  // In the broad phase, we re-sort each axis vec, and swaps signal a potential collision.
  struct extent
  {
    DERIVED_TYPE* go = nullptr;
    //int id = 0;
    float pos = 0; // extent (min or max) on this axis

    // Need this? So we only treat 2 of four cases as potential collisions??
    bool is_min = true; 
  };

  using extent_vec = std::vector<extent>;


  void one_axis(int axis, collision_vec& result, const DOUBLE_DISPATCHER& dd)
  {
    extent_vec& extents = m_extent_vecs[axis];
    one_axis(extents, result, dd);
  } 
 
  void one_axis(extent_vec& extents, collision_vec& result, const DOUBLE_DISPATCHER& dd)
  {
    // Insertion sort
    const int size = static_cast<int>(extents.size());
    for (int i = 0; i < size; i++)
    {
      int j = i;
      while (j > 0 && extents[j - 1].pos > extents[j].pos)
      {
        std::swap(extents[j - 1], extents[j]);
        // The objects referred to in the elements we just swapped are potential
        //  colliders
        DERIVED_TYPE* obj1 = extents[j - 1].go;
        DERIVED_TYPE* obj2 = extents[j].go;
        assert(obj1);
        assert(obj2);

        if (dd.has_handler(obj1, obj2))
        {
          result.push_back(make_coll_pair(obj1, obj2));
        }
        j--;
      } 
    }
  }

  void populate_extents(const game_objects& gos)
  {
    for (std::shared_ptr<game_object> go : gos)
    {
      assert(go);

      for (int axis = 0; axis < NUM_AXES; axis++)
      {
        extent_pair ep = make_extents_for_game_object_for_axis(dynamic_cast<const DERIVED_TYPE*>(go.get()), axis);
        add_extent(axis, ep.first);
        add_extent(axis, ep.second);
      }
    }
  }

  void add_extents(int axis, DERIVED_TYPE* go, float min_value, float max_value)
  {
    assert(go);
    add_extent(axis, extent{go, min_value, true});
    add_extent(axis, extent{go, max_value, false});
  }

  void add_extent(int axis, const extent& e)
  {
    assert(axis < NUM_AXES);
    add_extent(m_extent_vecs[axis], e);
  }

  const extent_vec& get_extent_vec(int axis) const 
  {
    assert(axis < NUM_AXES);
    return m_extent_vecs[axis];
  }

  extent_vec& get_extent_vec(int axis)
  {
    assert(axis < NUM_AXES);
    return m_extent_vecs[axis];
  }

  void add_extent(extent_vec& ev, const extent& e)
  {
    ev.push_back(e);
  }

  using extent_pair = std::pair<extent, extent>;
  extent_pair  make_extents_for_game_object_for_axis(const DERIVED_TYPE* go, int axis) const
  {
    assert(go);
    return extent_pair(
      extent{ const_cast<DERIVED_TYPE*>(go), get_min_axis_value(*go, axis), true },
      extent{ const_cast<DERIVED_TYPE*>(go), get_max_axis_value(*go, axis), false } );
  }

  float get_min_axis_value(const DERIVED_TYPE& go, int axis) const
  {
    switch (axis)
    {
    case 0:
      return go.get_pos().x;
    case 1:
      return go.get_pos().y;
    }
    assert(false && "axis out of range");
  }

  float get_max_axis_value(const DERIVED_TYPE& go, int axis) const
  {
    switch (axis)
    {
    case 0:
      return go.get_pos().x + go.get_size().x;
    case 1:
      return go.get_pos().y + go.get_size().y;
    }
    assert(false && "axis out of range");
  }

private:
  const game_objects* m_game_objects = nullptr;

  std::array<extent_vec, NUM_AXES> m_extent_vecs;

};


