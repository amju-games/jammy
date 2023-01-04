#pragma once

#include "jammy_game_object.h"

class parallax_bg : public jammy_game_object
{
public:
  parallax_bg();
  void draw(screen&) override;
  void update(float dt) override;

private:
  static constexpr int NUM_IMAGES = 1;
  sprite m_sprites[NUM_IMAGES];

  static constexpr int NUM_LAYERS = 2;
  vec2 m_bg_pos[NUM_LAYERS];
};

