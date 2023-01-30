#pragma once

#include <array>
#include "action.h"

class auto_repeat_suppressor
{
public:
  bool is_new_event(const keyboard_action& ka)
  {
    static_assert(static_cast<bool>(button_value::up) == false);
    static_assert(static_cast<bool>(button_value::down) == true);

    auto [ch, val](ka);
    bool is_key_down = static_cast<bool>(val);
    bool res = m_keys[ch] != is_key_down;
    m_keys[ch] = is_key_down;
    return res; 
  }

private:
  // Flag for each possible key value
  std::array<bool, 256> m_keys = {};
};

