#include "directory.h"
#include "globals.h"
#include "player_bullet.h"
#include "rock.h"
#include "sound_player.h"

void collision_player_bullet_rock(player_bullet* pb, rock* r)
{
  pb->set_is_alive(false); 
  pb->set_is_collidable(false);
  
  // Destroy rock, show explosion
  r->explode();
}

