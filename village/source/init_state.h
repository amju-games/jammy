#pragma once

#include "jammy_game_state.h"

class init_state : public jammy_game_state
{
public:
  void update([[maybe_unused]] float dt) override {}
  void draw() override {}
  void on_active() override;

private:
  // Use this level as background for states until player loads a level
  void load_attract_level();
};

