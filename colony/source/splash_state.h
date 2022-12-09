#pragma once

#include "jammy_game_state.h"

class splash_state : public jammy_game_state
{
public:
  splash_state();
  void update(float dt) override;
  void draw() override;
  void on_input(int input) override;

private:
  image m_image;
};

