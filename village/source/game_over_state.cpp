#include "blit.h"
#include "directory.h"
#include "game_over_state.h"
#include "globals.h"
#include "jammy_blend.h"
#include "player.h"
#include "resources.h"

game_over_state::game_over_state()
{
  // TODO proper image/animated scene
  // TODO Load anything configurable in on_active
  m_image = resources().get<image>(get_data_dir() + "doodles.png");
}

void game_over_state::update(float dt)
{
  // play_state::update(dt) does stuff we don't want/need.
  the_game.update_game_objects(dt);
  // Maybe coll det?
  jammy_game_state::update(dt); // update fps counter

  update_time_to_next_state();
}

void game_over_state::update_time_to_next_state()
{
  static const float WAIT_TIME = 4.f;
  if (get_time_in_state() > WAIT_TIME)
  {
    int score = the_player_progress.get_score();
    if (the_hi_scores.is_hi_score(score))
    {   
      the_game.set_game_state(the_enter_hi_score_state);
    }   
    else
    {   
      the_game.set_game_state(the_show_hi_scores_state);
    }   
  }
}

void game_over_state::draw()
{
  play_state_base::draw();

  blit<jb_overwrite>(m_image, the_screen, 0, 0); 
  
  get_font()->draw<jb_font_mask>(the_screen, 46, 25, "GAME OVER");
}

