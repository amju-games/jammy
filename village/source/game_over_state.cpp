#include "blit.h"
#include "directory.h"
#include "game_over_state.h"
#include "globals.h"
#include "jammy_blend.h"
#include "player.h"
#include "resources.h"

void draw_centred(int y, const std::string& str)
{
  const float CHAR_W = 4;
  the_font.draw<jb_font_mask>(the_screen, 63 - str.length() * CHAR_W / 2, y, str);
}

game_over_state::game_over_state()
{
  // TODO proper image/animated scene
  m_image = resources().get<image>(get_data_dir() + "doodles.png");

  m_human_ss.set_image(resources().get<image>(get_data_dir() + "Human_all.png"));
  m_human_ss.set_num_cells(9, 1);
}

static float t = 0;
static const float WAIT_TIME = 10.f;

void game_over_state::on_active()
{
  t = 0;
}

void game_over_state::update(float dt)
{
  t += dt;
  if (t > WAIT_TIME)
  {
    int score = the_player_progress.get_score();
    if (the_hi_score_table.is_hi_score(score))
    {   
      the_game.set_game_state(the_splash_state);
      //the_game.set_game_state(the_enter_hi_score_state);
    }   
    else
    {   
      the_game.set_game_state(the_splash_state);
    }   
  }
}

void game_over_state::draw()
{
  blit<jb_overwrite>(m_image, the_screen, 0, 0); 
  
  the_font.draw<jb_font_mask>(the_screen, 46, 25, "GAME OVER");

  int saved = 0; //the_play_state->get_player()->get_num_humans_saved();
  std::string str = std::to_string(saved);
 
  if (t > 1.2f) 
  {
    draw_centred(45, "HUMANS SAVED: " + str);

    int h = saved;
    for (int i = 0; i < h; i++)
    {
      m_human_ss.draw_cell<jb_mask>(the_screen, 0, 63 - (i * 20) / 2, 65);
    }
  }

  int score = the_player_progress.get_score();
  str = std::to_string(score);

  if (t > .5f)
  {
    draw_centred(35, "SCORE: " + str);
  }

}

