#include "collision_player_rock.h"

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
    r->explode();

    // TODO Do this in lose_life
    // the_sound_player->play_wav(get_data_dir() + "sounds/Explosion2.wav");
  }
}

