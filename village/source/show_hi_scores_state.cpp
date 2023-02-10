#include "config_file.h"
#include "globals.h"
#include "jammy_blend.h"
#include "resources.h"
#include "show_hi_scores_state.h"

void show_hi_scores_state::on_active() 
{
  jammy_game_state::on_active();

  std::shared_ptr<config_file> config = resources().get<config_file>("config.txt");
  m_y_spacing = config->get_int("show_hi_score_state::m_y_spacing");
  m_y_start = config->get_int("show_hi_score_state::m_y_start");
}

void show_hi_scores_state::draw() 
{
  int num_scores = the_hi_scores.get_num_hi_scores();

  for (int i = 0; i < num_scores; i++)
  {
    int score = 0;
    std::string name;
    the_hi_scores.get_hi_score(num_scores - i - 1, score, name);
    float y = i * m_y_spacing + m_y_start;
    std::string s = std::to_string(i + 1) + ". " + name + std::to_string(score);
    the_font.draw<jb_font_mask>(the_screen, 10, y, s);
  }

  jammy_game_state::draw();
}

void show_hi_scores_state::update(float dt)
{
  jammy_game_state::update(dt);

  if (get_time_in_state() > 20.f)
  {
    the_game.set_game_state(the_splash_state);
  }
}

void show_hi_scores_state::on_keyboard_action(const keyboard_action& ka) 
{
  auto [key, value](ka);
  if (   value == button_value::down
      && get_time_in_state() > 2.f)
  {
    the_game.set_game_state(the_splash_state);
  }
}


