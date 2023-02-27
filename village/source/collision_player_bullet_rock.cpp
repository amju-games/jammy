#include "player_bullet.h"
#include "rock.h"

void collision_player_bullet_rock(player_bullet* pb, rock* r)
{
  pb->set_is_updatable(false); 
  pb->set_is_collidable(false);
  pb->set_is_visible(false);
  
  // Destroy rock, show explosion
  r->explode();
}

