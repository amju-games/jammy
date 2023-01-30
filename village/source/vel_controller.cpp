#include <iostream>

#include "vel_controller.h"

void vel_controller::on_dir_button_action(const dir_button_action& ba) 
{
  const auto [button, value] = ba;
  m_button_values[static_cast<int>(button)] = value;
}

void vel_controller::on_joystick_action(const joystick_action& ja) 
{
std::cout << "Joystick x: " << ja.first << " y: " << ja.second << "\n";
  m_joystick_action = ja;
}

void vel_controller::reset()
{
  for (auto& bv : m_button_values)
  {
    bv = button_value::up;
  }
}

bool vel_controller::button(dir_button_name bn) const
{ 
  return m_button_values[static_cast<int>(bn)] == button_value::down;
}  

vec2 vel_controller_flying::convert_active_buttons_to_vel() const
{
  // No accel: immediately set vel according to buttons. 
  float x = 0, y = 0;
  if (button(dir_button_name::right)) x += 1.f;
  if (button(dir_button_name::left)) x += -1.f;
  if (button(dir_button_name::up)) y -= 1.f;
  if (button(dir_button_name::down)) y += 1.f;
  if (m_up_is_positive_y) y = -y;

  return vec2(x, y);
}

vec2 vel_controller_flying::convert_joystick_action_to_vel() const
{
  const float DEAD_ZONE = 0.05f; // TODO CONFIG

  vec2 joystick = vec2(m_joystick_action.first, m_joystick_action.second);
  if (fabs(joystick.x) < DEAD_ZONE) joystick.x = 0;
  if (fabs(joystick.y) < DEAD_ZONE) joystick.y = 0;
  return joystick;
}

void vel_controller_flying::update([[maybe_unused]] float dt) 
{
  const float DEAD_ZONE = 0.05f; // TODO CONFIG

  vec2 vel = convert_active_buttons_to_vel();

  vel += convert_joystick_action_to_vel();

  vel.x = std::clamp(vel.x, -1.f, 1.f);
  vel.y = std::clamp(vel.y, -1.f, 1.f);

  vel *= m_max_speed; // now vel x and y are in range -min_speed .. min_speed

  // Only update x/y component if action is enabled in that direction, otherwise
  //  decelerate.

  if (fabs(vel.x) > DEAD_ZONE) m_vel.x = vel.x;
  if (fabs(vel.y) > DEAD_ZONE) m_vel.y = vel.y;

  const float DECELERATION = -1.f; // TODO CONFIG
  m_acc = m_vel * DECELERATION;
}

