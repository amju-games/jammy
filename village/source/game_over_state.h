#pragma once

#include "image.h"
#include "jammy_game_state.h"
#include "sprite_sheet.h"

class game_over_state : public jammy_game_state
{
public:
  game_over_state();
  void update(float dt) override;
  void draw() override;
  void on_input(int input) override;
  void on_active() override;
  
private:
  image m_image;
  sprite_sheet m_human_ss;
};

