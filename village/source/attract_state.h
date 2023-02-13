#pragma once

#include "play_state.h"

class attract_state : public play_state_base
{
public:
  void on_active() override;
  void update(float dt) override;
  bool on_keyboard_action(const keyboard_action&) override;
  bool on_game_controller_button_action(const game_controller_button_action&) override;

private:
  float m_timeout = 0;
};

