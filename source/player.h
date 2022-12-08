#pragma once

#include "jammy_game_object.h"

// 8-way directional movement
const int MOVE_UP =    0x01;
const int MOVE_DOWN =  0x02;
const int MOVE_LEFT =  0x04;
const int MOVE_RIGHT = 0x08;

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

