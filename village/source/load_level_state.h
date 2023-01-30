#pragma once

#include "font.h"
#include "jammy_game_state.h"

class load_level_state : public jammy_game_state
{
public:
  load_level_state();
  void update(float dt) override;
  void draw() override;
  void on_active() override; 

  void on_keyboard_action(const keyboard_action&) override;
  void on_game_controller_button_action(const game_controller_button_action&) override;

private:
  font m_big_font; // TODO font manager?? This is cheap to create tho
};

