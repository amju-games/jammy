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
  m_x_title = config->get_int("show_hi_score_state::m_x_title");
  m_y_title = config->get_int("show_hi_score_state::m_y_title");
  m_scale_title = config->get_float("show_hi_score_state::m_scale_title");
  m_score_time = config->get_float("show_hi_score_state::m_score_time");
}

void show_hi_scores_state::draw() 
{
  play_state_base::draw();

  get_font()
    .set_scale(m_scale_title)
    .draw<jb_font_mask>(the_screen, m_x_title, m_y_title, "HI SCORES") // TODO localise
     // TODO Centred
    .set_scale(1.f);

  int num_scores = the_hi_scores.get_num_hi_scores();

  for (int i = 0; i < num_scores; i++)
  {
    float time_to_show_score = static_cast<float>(i) * m_score_time;
    if (get_time_in_state() < time_to_show_score)
    {
      break;
    }

    int score = 0;
    std::string name;
    the_hi_scores.get_hi_score(num_scores - i - 1, score, name);
    float y = i * m_y_spacing + m_y_start;
    std::string s = std::to_string(i + 1) + "."; 
    get_font()
      .draw<jb_font_mask>(the_screen, m_x_number, y, s)
      .draw<jb_font_mask>(the_screen, m_x_score, y, std::to_string(score))
      .draw<jb_font_mask>(the_screen, m_x_name, y, name);
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


