#include "blit.h"
#include "directory.h"
#include "enter_hi_score_state.h"
#include "globals.h"
#include "jammy_blend.h"
#include "player.h"
#include "resources.h"

enter_hi_score_state::enter_hi_score_state()
{
  m_image = resources().get<image>(get_data_dir() + "Background.png");
}

static float t = 0;
static const float WAIT_TIME = 5.f;

void enter_hi_score_state::on_active()
{
  t = 0;
}

void enter_hi_score_state::update(float dt)
{
  t += dt;
  if (t > WAIT_TIME)
  {
    int score = the_play_state->get_player()->get_score();
    if (the_hi_score_table.is_hi_score(score))
    {
      the_game.set_game_state(the_enter_hi_score_state);
    }
    else
    {
      the_game.set_game_state(the_splash_state);
    }
  }
}

void enter_hi_score_state::draw()
{
  blit<jb_overwrite>(m_image, the_screen, 0, 0); 
  
  the_font.draw<jb_font_mask>(the_screen, 46, 30, "GAME OVER");

  int score = the_play_state->get_player()->get_score();
  std::string str = std::to_string(score);
  const float CHAR_W = 4;
  the_font.draw<jb_font_mask>(the_screen, 63 - (6 + str.length()) * CHAR_W / 2, 80, "SCORE: " + str);
}

void enter_hi_score_state::on_input(int input) 
{
  the_game.set_game_state(the_splash_state);
}
  
