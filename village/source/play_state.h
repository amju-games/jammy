#pragma once

#include <vector>
#include "jammy_collision_mgr.h"
#include "jammy_game_state.h"
#include "level.h"

class jammy_game_object;

class play_state : public jammy_game_state
{
public:
  play_state();
  void update(float dt) override;
  void draw() override;
  void on_active() override;
  void on_deactive() override;

  void on_dir_button_action(const dir_button_action&) override;
  void on_joystick_action(const joystick_action&) override;
  void on_keyboard_action(const keyboard_action&) override;
  void on_game_controller_button_action(const game_controller_button_action&) override;

protected:
  void col_det();
  void draw_radar();
  void draw_lives();
  void draw_blip(jammy_game_object* h, int cell);

protected:
  p_image m_radar;
  sprite_sheet m_blips;
  p_image m_life_empty;
  p_image m_life_full;

  jammy_collision_mgr m_collision_mgr;
};

