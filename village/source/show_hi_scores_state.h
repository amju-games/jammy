#pragma once

#include "play_state.h"

class show_hi_scores_state : public play_state_base
{
public:
  void on_active() override;

  void draw() override;
  void update(float dt) override;

  bool on_keyboard_action(const keyboard_action&) override;

private:
  int m_y_start = 0;
  int m_y_spacing = 0;
  int m_x_number = 0;
  int m_x_score = 0;
  int m_x_name = 0;
  float m_timeout = 0;
  int m_x_title = 0;
  int m_y_title = 0;
  float m_scale_title = 1.f;
};

