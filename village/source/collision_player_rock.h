#pragma once

#include "player.h"
#include "rock.h"

void collision_player_rock(player* p, rock* r)
{
  if (p->is_immune())
  {
    p->keep_immune();
  }
  else
  {
    p->lose_life();
    // Destroy rock?

    // TODO Do this in lose_life
    // the_sound_player->play_wav(get_data_dir() + "sounds/Explosion2.wav");
  }
}

