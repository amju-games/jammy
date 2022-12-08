#pragma once

#include "jammy_game_state.h"

class player;

class play_state : public jammy_game_state
{
public:
  void update(float dt) override;
  void draw() override;
  void on_input(int input) override;
  void on_active() override;
  void on_deactive() override;

protected:
  void col_det();

protected:
  player* m_player = nullptr;
};

