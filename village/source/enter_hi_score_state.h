#pragma once

#include "image.h"
#include "jammy_game_state.h"

class enter_hi_score_state : public jammy_game_state
{
public:
  enter_hi_score_state();
  void update(float dt) override;
  void draw() override;
  void on_active() override;

  bool on_keyboard_action(const keyboard_action&) override;
  bool on_game_controller_button_action(const game_controller_button_action&) override;
  
private:
  p_image m_image;
};

