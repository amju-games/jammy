#include <iostream>

#include "moon.h"
#include "player_bullet.h"

void collision_player_bullet_moon(player_bullet* pb, moon* m)
{
std::cout << "Collide pb and moon\n";

  pb->set_is_updatable(false); 
  pb->set_is_collidable(false);

  m->on_shot_by_player(pb);  
}

