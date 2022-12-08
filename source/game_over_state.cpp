#include "directory.h"
#include "game_over_state.h"
#include "globals.h"
#include "player.h"

void draw_centred(int y, const std::string& str)
{
  const float CHAR_W = 4;
  the_font.draw(the_screen, 63 - str.length() * CHAR_W / 2, y, str);
}

game_over_state::game_over_state()
{
  m_image.load(get_data_dir() + "Background.png", the_global_palette);

  m_human_ss.load(get_data_dir() + "Human_all.png", the_global_palette);
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
    int score = the_play_state->get_player()->get_score();
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
  m_image.blit(the_screen, 0, 0); 
  
  the_font.draw(the_screen, 46, 25, "GAME OVER");

  int saved = the_play_state->get_player()->get_num_humans_saved();
  std::string str = std::to_string(saved);
 
  if (t > 1.2f) 
  {
    draw_centred(45, "HUMANS SAVED: " + str);

    int h = saved;
    for (int i = 0; i < h; i++)
    {
      m_human_ss.draw_cell(the_screen, 0, 63 - (i * 20) / 2, 65);
    }
  }

  int score = the_play_state->get_player()->get_score();
  str = std::to_string(score);

  if (t > .5f)
  {
    draw_centred(35, "SCORE: " + str);
  }

}

void game_over_state::on_input(int input) 
{
  // Prevent accidentally clicking through
  if (t > 3.f)
  { 
    t += WAIT_TIME; // so we go to next state on update
  }
}
  
