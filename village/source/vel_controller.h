#pragma once

#include <array>
#include "action.h"
#include "vec2.h"

class vel_controller
{
public:
  virtual ~vel_controller() = default;

  virtual void update(float dt) = 0;

  void on_dir_button_action(const dir_button_action& ba) 
  {
    const auto [button, value] = ba;
    m_button_values[static_cast<int>(button)] = value;
  }

  void on_joystick_action(const joystick_action& ja) 
  {
  }

  void reset()
  {
    for (auto& bv : m_button_values)
    {
      bv = button_value::up;
    }
  }

  void set_max_speed(float max_speed) { m_max_speed = max_speed; }

  const vec2& get_vel() const { return m_vel; }
  const vec2& get_acc() const { return m_acc; }

protected:
  // Useful to get the cast out of the way
  bool button(dir_button_name bn) const
  { 
    return m_button_values[static_cast<int>(bn)] == button_value::down;
  }  

protected:
  // We will calculate these values in update()
  vec2 m_vel;
  vec2 m_acc;

  float m_max_speed = 1.f;

  // Store inputs for use in update()
  std::array<button_value, 4> m_button_values = {};

  // Up/down flag
  bool m_up_is_positive_y = false;
};


// Update player vel in response to button actions
// TODO Policy-based vel controller: use policies for accel and decel.
class vel_controller_buttons_no_accel : public vel_controller
{
public:
  void update([[maybe_unused]] float dt) override
  {
    // No accel: immediately set vel according to buttons. 
    float x = 0, y = 0;
    if (button(dir_button_name::right)) x += 1.f;
    if (button(dir_button_name::left)) x += -1.f;
    if (button(dir_button_name::up)) y -= 1.f;
    if (button(dir_button_name::down)) y += 1.f;
    if (m_up_is_positive_y) y = -y;
    vec2 vel(x, y);
    if (x != 0 || y != 0) vel = normalise(vel);    
    vel *= m_max_speed;
    m_vel = vel;
  }
};

