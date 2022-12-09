#pragma once

#include "jammy_game_object.h"

class parallax_bg : public jammy_game_object
{
public:
  parallax_bg();
  void draw(screen&) override;
  void update(float dt) override;

private:
  static const int NUM_IMAGES = 1;
  image m_images[NUM_IMAGES];
  vec2 m_bg_pos[NUM_IMAGES];
};

