#include "collision_player_rock.h"
#include "collision_player_bullet_rock.h"
#include "jammy_collision_mgr.h"
#include "player.h"
#include "player_bullet.h"
#include "rock.h"

void populate_collision_funcs(jammy_collision_mgr& cm)
{
  cm.add_handler<player, rock>(collision_player_rock);
  cm.add_handler<player_bullet, rock>(collision_player_bullet_rock);
}


