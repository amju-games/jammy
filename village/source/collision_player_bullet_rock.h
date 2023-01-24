#pragma once

#include <iostream>

#include "player_bullet.h"
#include "rock.h"

void collision_player_bullet_rock(player_bullet* pb, rock* r)
{
std::cout << "Bullet hits rock!\n";
  pb->set_is_alive(false); 
  pb->set_is_collidable(false);
  
  // Destroy rock, show explosion
  r->set_is_alive(false);
  r->set_is_collidable(false);

  // the_sound_player->play_wav(get_data_dir() + "sounds/Explosion2.wav");
}

