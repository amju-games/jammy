#pragma once

#include "image.h"
#include "jammy_game_state.h"

class enter_hi_score_state : public jammy_game_state
{
public:
  enter_hi_score_state();
  void update(float dt) override;
  void draw() override;
  void on_input(int input) override;
  void on_active() override;
  
private:
  image m_image;
};

