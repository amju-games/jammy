# pragma once

#include "game_state.h"

class jammy_game_state : public game_state
{
public:
  void on_active() override;

  void draw() override;
  void update(float dt) override;

  bool on_keyboard_action(const keyboard_action&) override; 

  float get_time_in_state() const { return m_time_in_state; }

private:
  float m_time_in_state = 0;
};

