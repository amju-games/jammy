#pragma once

#include "jammy_game_object.h"

class parallax_bg : public jammy_game_object
{
public:
  parallax_bg();
  void draw(ref_image) override;
  void update(float dt) override;

  bool load(const std::string& filename);

private:
  static constexpr int NUM_IMAGES = 1;
  sprite m_sprites[NUM_IMAGES];

  static constexpr int NUM_LAYERS = 2;
  vec2 m_bg_pos[NUM_LAYERS];
};

