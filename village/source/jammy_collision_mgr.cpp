#include <iostream>
#include "collision_player_human.h"
#include "collision_player_rock.h"
#include "jammy_collision_mgr.h"
#include "player.h"
#include "player_bullet.h"

void populate_collision_funcs(jammy_collision_mgr& cm)
{
  // TODO TEMP TEST
  cm.add_handler<player, player_bullet>(
    [](player* p, player_bullet* pb)
    {
      std::cout << "Ouch!\n";
    }
  );

  cm.add_handler<player, rock>(collision_player_rock);
  cm.add_handler<player, human>(collision_player_human);
}


