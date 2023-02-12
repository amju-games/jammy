#include "config_file.h"
#include "globals.h"
#include "jammy_blend.h"
#include "resources.h"
#include "show_hi_scores_state.h"

void show_hi_scores_state::on_active() 
{
  play_state_base::on_active();

  std::shared_ptr<config_file> config = resources().get<config_file>("config.txt");
  m_y_spacing = config->get_int("show_hi_score_state::m_y_spacing");
  m_y_start = config->get_int("show_hi_score_state::m_y_start");
  m_timeout = config->get_float("show_hi_score_state::m_timeout");
  m_x_number = config->get_int("show_hi_score_state::m_x_number");
  m_x_score = config->get_int("show_hi_score_state::m_x_score");
  m_x_name = config->get_int("show_hi_score_state::m_x_name");
}

void show_hi_scores_state::draw() 
{
  play_state_base::draw();

  get_font()->draw<jb_font_mask>(the_screen, 0, 50, "HI SCORES"); // TODO scale up font

  int num_scores = the_hi_scores.get_num_hi_scores();

  for (int i = 0; i < num_scores; i++)
  {
    int score = 0;
    std::string name;
    the_hi_scores.get_hi_score(num_scores - i - 1, score, name);
    float y = i * m_y_spacing + m_y_start;
    std::string s = std::to_string(i + 1) + "."; 
    get_font()->draw<jb_font_mask>(the_screen, m_x_number, y, s);
    get_font()->draw<jb_font_mask>(the_screen, m_x_score, y, std::to_string(score));
    get_font()->draw<jb_font_mask>(the_screen, m_x_name, y, name);
  }

  jammy_game_state::draw(); // draw fps
}

void show_hi_scores_state::update(float dt)
{
  play_state_base::update(dt);

  if (get_time_in_state() > m_timeout)
  {
    the_game.set_game_state(the_splash_state);
  }
}

bool show_hi_scores_state::on_keyboard_action(const keyboard_action& ka) 
{
  if (play_state_base::on_keyboard_action(ka))
  {
    return true; // consumed
  }

  auto [key, value](ka);
  if (   value == button_value::down
      && key == ' '
      && get_time_in_state() > 1.f) // TODO CONFIG
  {
    the_game.set_game_state(the_splash_state);
    return true;
  }
  return false;
}


