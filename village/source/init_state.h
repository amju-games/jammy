#pragma once

#include "jammy_game_state.h"

class init_state : public jammy_game_state
{
public:
  void update(float dt) override {}
  void draw() override {}
  void on_active() override;
};

