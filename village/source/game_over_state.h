#pragma once

#include "image.h"
#include "play_state.h"
#include "sprite_sheet.h"

class game_over_state : public play_state 
{
public:
  game_over_state();
  void update(float dt) override;
  void draw() override;
  void on_active() override;
  
private:
  p_image m_image;
};

