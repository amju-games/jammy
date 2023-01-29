#pragma once

#include <iostream>

#include <array>
#include "vec2.h"

// Each enum value * each controller type corresponds to a cell range, right?

// Hmm this is very game-specific - maybe better to map strings to anim_info and not
//  have this enum?
enum class anim_state
{
  idle,
  idle_left,
  idle_right,
  face_left,
  face_right,

  num_anim_states // surely there's a better way?
};


// Sprite sheet info related to a given anim_state
struct anim_info
{
  int m_cell_min = 0;
  int m_cell_max = 0; // inclusive
  bool m_is_looping = false;
  float m_cell_max_time;
  // Should we point to the sprite sheet here? I think so
  
};


class anim_controller
{
public:
  virtual ~anim_controller() = default;

  virtual anim_state get_anim_state() = 0;
  virtual void update(float dt) = 0;
  virtual void set_vel(const vec2& vel) = 0;
  virtual void set_anim_info(anim_state, const anim_info&) = 0;
  virtual const anim_info& get_anim_info() const = 0;
};


class anim_controller_stores_anim_info : public anim_controller
{
public:
  void set_anim_info(anim_state as, const anim_info& ai) override
  {
    m_anim_info[static_cast<int>(as)] = ai;
  }
 
protected:
  std::array<anim_info, static_cast<int>(anim_state::num_anim_states)> m_anim_info = {};
};


class anim_controller_flying : public anim_controller_stores_anim_info
{
public:
  anim_state get_anim_state() override
  {
    return m_anim_state;
  }

  const anim_info& get_anim_info() const override
  {
    return m_anim_info[static_cast<int>(m_anim_state)];
  }
 
  void update(float dt) override
  {
    const float DEAD_ZONE = 0; //0.5f;

    anim_state new_anim_state = anim_state::idle;
    if (m_vel.x < -DEAD_ZONE)
    {
      new_anim_state = anim_state::face_left;
    }
    else if (m_vel.x > DEAD_ZONE)
    {
      new_anim_state = anim_state::face_right;
    }
    // If we change anim_state, we can set the start cell in the new range to match
    //  the cell we were at in the previous range.

    m_anim_state = new_anim_state;
  }

  void set_vel(const vec2& vel) override
  {
    m_vel = vel;
  }

private:
  anim_state m_anim_state = anim_state::idle;
  vec2 m_vel;
};

