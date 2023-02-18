#include "blit.h"
#include "config_file.h"
#include "directory.h"
#include "game_over_state.h"
#include "globals.h"
#include "jammy_blend.h"
#include "player.h"
#include "resources.h"

game_over_state::game_over_state()
{
}

void game_over_state::on_active()
{
  play_state_base::on_active();

  std::shared_ptr<config_file> config = resources().get<config_file>("config.txt");
}

void game_over_state::update(float dt)
{
  play_state_base::update(dt); // does coll det, but that's ok

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

  // TODO Centred  
  get_font()
    .set_scale(4.f)
    .draw<jb_font_mask>(the_screen, 46, 25, "GAME OVER")
    .set_scale(1.f);
}

