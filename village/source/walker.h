#pragma once

#include "jammy_game_object.h"

class walker : public jammy_game_object
{
public:
  void update(float dt) override;
};

