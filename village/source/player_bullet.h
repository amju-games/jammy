#pragma once

#include "jammy_game_object.h"

class player_bullet : public jammy_game_object
{
public:
  player_bullet(const vec2& pos, const vec2& dir);
};

