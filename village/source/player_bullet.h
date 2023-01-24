#pragma once

#include "image_colour_xform.h"
#include "jammy_game_object.h"
#include "player.h"

class player_bullet : public jammy_game_object
{
public:
  player_bullet();

  void update(float dt) override;

  void fire(std::shared_ptr<player>& player);

private:
  std::shared_ptr<image_colour_xform> m_colour_decorator;
};

