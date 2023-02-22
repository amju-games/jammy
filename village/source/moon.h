#pragma once

#include "jammy_game_object.h"

class player_bullet;

class moon : public jammy_game_object
{
public:
  moon();
  void on_shot_by_player(player_bullet* pb);

};

