#pragma once

#include "colour.h"
#include "font.h"
#include "jammy_game_state.h"

class load_level_state : public jammy_game_state
{
public:
  void update(float dt) override;
  void draw() override;
  void on_active() override; 

  bool on_keyboard_action(const keyboard_action&) override;
  bool on_game_controller_button_action(const game_controller_button_action&) override;

private:
  colour m_bg_colour;
};

