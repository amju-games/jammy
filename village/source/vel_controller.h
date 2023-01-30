#pragma once

#include <array>
#include "action.h"
#include "sign.h"
#include "vec2.h"

class vel_controller
{
public:
  virtual ~vel_controller() = default;

  virtual void update(float dt) = 0;

  void on_dir_button_action(const dir_button_action& ba);

  void on_joystick_action(const joystick_action& ja);

  void reset();

  void set_max_speed(float max_speed) { m_max_speed = max_speed; }

  void set_vel(const vec2& vel) { m_vel = vel; }
  void set_acc(const vec2& acc) { m_acc = acc; }

  const vec2& get_vel() const { return m_vel; }
  const vec2& get_acc() const { return m_acc; }

protected:
  // Useful to get the cast out of the way
  bool button(dir_button_name bn) const;

protected:
  // We will make changes to these values in update()
  vec2 m_vel;
  vec2 m_acc;

  float m_max_speed = 1.f;

  // Store inputs for use in update()
  std::array<button_value, 4> m_button_values = {};

  // Up/down flag
  bool m_up_is_positive_y = false;

  joystick_action m_joystick_action = {};
};


// Update player vel in response to button actions

class vel_controller_flying : public vel_controller
{
public:
  void update([[maybe_unused]] float dt) override;

private:
  // Return vec where x and y are in range -1 .. 1
  vec2 convert_active_buttons_to_vel() const;
  vec2 convert_joystick_action_to_vel() const;

};

