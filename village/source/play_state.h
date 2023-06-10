#pragma once

#include "play_state_base.h"

class play_state : public play_state_base
{
public:
  void update(float dt) override;
  void draw() override;
  void on_active() override;
  void on_deactive() override;

  bool on_dir_button_action(const dir_button_action&) override;
  bool on_joystick_action(const joystick_action&) override;
  bool on_keyboard_action(const keyboard_action&) override;
  bool on_game_controller_button_action(const game_controller_button_action&) override;

protected:
  virtual void check_for_game_over();
  virtual void check_for_level_completed();

  virtual void go_to_next_level();
};

