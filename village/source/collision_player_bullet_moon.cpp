#include <iostream>

#include "moon.h"
#include "player_bullet.h"

void collision_player_bullet_moon(player_bullet* pb, moon* m)
{
  pb->set_is_updatable(false); 
  pb->set_is_collidable(false);
  pb->set_is_visible(false);

  m->on_shot_by_player(pb);  
}

