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

  the_sound_player->play_wav(get_data_dir() + "sounds/Explosion2.wav");

  the_play_state->get_player()->add_score(r->get_score());
  
  the_play_state->dec_num_rocks();
}

