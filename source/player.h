#pragma once

#include "jammy_game_object.h"

class player : public jammy_game_object
{
public:

  player();

  void update(float dt) override;

  // * move *
  // Move in the given direction, a bitfield combination of 
  //  directions.
  void move(int move_dir);

};

