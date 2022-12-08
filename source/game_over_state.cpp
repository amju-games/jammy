#include "directory.h"
#include "game_over_state.h"
#include "globals.h"
#include "player.h"

game_over_state::game_over_state()
{
  m_image.load(get_data_dir() + "Background.png", the_global_palette);
}

static float t = 0;
static const float WAIT_TIME = 5.f;

void game_over_state::on_active()
{
  t = 0;
}

void game_over_state::update(float dt)
{
  t += dt;
  if (t > WAIT_TIME)
  {
    the_game.set_game_state(the_splash_state);
  }
}

void game_over_state::draw()
{
  m_image.blit(the_screen, 0, 0); 
  
  the_font.draw(the_screen, 46, 30, "GAME OVER");

  int score = the_play_state->get_player()->get_score();
  std::string str = std::to_string(score);
  const float CHAR_W = 4;
  the_font.draw(the_screen, 63 - (6 + str.length()) * CHAR_W / 2, 80, "SCORE: " + str);
}

void game_over_state::on_input(int input) 
{
  the_game.set_game_state(the_splash_state);
}
  
