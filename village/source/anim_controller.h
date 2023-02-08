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
};


class anim_controller
{
public:
  virtual ~anim_controller() = default;

  virtual void set_anim_state(anim_state) = 0;
  virtual anim_state get_anim_state() = 0;
  virtual void update(float dt) = 0;
  virtual void set_vel(const vec2& vel) = 0;

  // Return the cell, which will be in the range of the currently active
  //  anim state/transition.
  virtual int get_cell() const = 0;

  // Store info for one animation
  virtual void set_anim_info(anim_state, const anim_info&) = 0;

  // Set a transitional state - doesn't loop?
//  virtual void set_transition(anim_state, anim_state, const anim_info&) = 0;

  // Return true if a transition between two anim states has been defined.
  // If there is one, we go through it when changing state, otherwise we immediately
  //  go to the new state. 
//  virtual bool has_transition(anim_state, anim_state) const = 0;

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
  void set_anim_state(anim_state as) override
  {
    // TODO Set future state to as, so we transition to it
    m_anim_state = as;
  }
 
  anim_state get_anim_state() override
  {
    return m_anim_state;
  }

  const anim_info& get_anim_info() const override
  {
    return m_anim_info[static_cast<int>(m_anim_state)];
  }
 
  int get_cell() const override
  {
    return m_cell;
  }

  void update(float dt) override;

  void update_cell(float dt);

  void set_vel(const vec2& vel) override
  {
    m_vel = vel;
  }

private:
  anim_state m_anim_state = anim_state::idle;
  [[maybe_unused]] anim_state m_next_anim_state = anim_state::idle;
  vec2 m_vel;
  int m_cell = 0;
  float m_cell_time = 0.f;
};

