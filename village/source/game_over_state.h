#pragma once

#include "image.h"
#include "play_state.h"
#include "sprite_sheet.h"

class game_over_state : public play_state 
{
public:
  game_over_state();
  void update(float dt) override;
  void draw() override;

  void on_dir_button_action(const dir_button_action&) override {}
  void on_joystick_action(const joystick_action&) override {}
  void on_keyboard_action(const keyboard_action&) override {}
  void on_game_controller_button_action(const game_controller_button_action&) override {}

private:
  void update_time_to_next_state();
  
private:
  p_image m_image;
};

