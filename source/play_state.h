#pragma once

#include "jammy_game_state.h"

class play_state : public jammy_game_state
{
public:
  void update(float dt) override;
  void draw() override;
  void on_input(int input) override;
};

