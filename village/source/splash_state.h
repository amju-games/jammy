#pragma once

#include "jammy_game_state.h"

class splash_state : public jammy_game_state
{
public:
  splash_state();
  void update(float dt) override;
  void draw() override;

  void on_keyboard_action(const keyboard_action&) override;
  void on_game_controller_button_action(const game_controller_button_action&) override;

private:
  p_image m_image;
};

