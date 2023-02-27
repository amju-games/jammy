#pragma once

#include "jammy_game_object.h"

class player_bullet;

class moon : public jammy_game_object
{
public:
  moon();
  void on_shot_by_player(player_bullet* pb);

private:
  vec2 find_contact_point(player_bullet* pb);
  vec2 find_contact_point_from_left(player_bullet* pb);
  vec2 find_contact_point_from_right(player_bullet* pb);
  vec2 get_rel_pos(player_bullet* pb);

};

