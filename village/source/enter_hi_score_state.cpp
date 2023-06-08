#include "blit.h"
#include "enter_hi_score_state.h"
#include "globals.h"
#include "jammy_blend.h"
#include "player.h"
#include "resources.h"

enter_hi_score_state::enter_hi_score_state()
{
  m_image = resources().get<image>("Splash_Screen.png");
}

static const float WAIT_TIME = 5.f;

void enter_hi_score_state::on_active()
{
  jammy_game_state::on_active();

  // Set the high score in the high score table, save it out.
  int score = the_player_progress.get_score();
  std::string name = "JULIET"; // TODO Enter name or initials
  the_hi_scores.store(score, name);
  the_hi_scores.save(hi_scores_filename());
}

void enter_hi_score_state::update(float dt)
{
  jammy_game_state::update(dt);

  if (get_time_in_state() > WAIT_TIME)
  {
    the_game.set_game_state(the_splash_state);
  }
}

void enter_hi_score_state::draw()
{
  blit<jb_overwrite>(m_image, the_screen, 0, 0); 
  
  get_font().draw<jb_font_mask>(the_screen, 46, 30, "HI SCORE");

  int score = the_player_progress.get_score();
  std::string str = std::to_string(score);
  const float CHAR_W = 4;
  get_font().draw<jb_font_mask>(the_screen, 63 - (6 + str.length()) * CHAR_W / 2, 80, "SCORE: " + str);
}

bool enter_hi_score_state::on_keyboard_action(const keyboard_action& ka)
{
  // TODO Player types name

  auto [key, value](ka);
  if (value == button_value::down)
  {
    the_game.set_game_state(the_show_hi_scores_state);
    return true;
  }
  
  return true;
}

bool enter_hi_score_state::on_game_controller_button_action(
  const game_controller_button_action& gcba) 
{
  // TODO 
  auto [button, value](gcba);
  if (value == button_value::down)
  {
    the_game.set_game_state(the_show_hi_scores_state);
  }
  return true;
}
 
