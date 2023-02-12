#pragma once

#include "jammy_game_state.h"

class new_game_state : public jammy_game_state
{
public:
  void on_active() override;
  void update(float dt) override;
};

