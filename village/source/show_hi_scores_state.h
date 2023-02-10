#pragma once

#include "jammy_game_state.h"

class show_hi_scores_state : public jammy_game_state
{
public:
  void on_active() override;

  void draw() override;
  void update(float dt) override;

  void on_keyboard_action(const keyboard_action&) override;

private:
  int m_y_start = 0;
  int m_y_spacing = 0;
};

