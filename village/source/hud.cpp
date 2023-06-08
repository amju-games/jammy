#include "blit.h"
#include "config_file.h"
#include "directory.h"
#include "globals.h"
#include "hud.h"
#include "image.h"
#include "jammy_blend.h"
#include "jammy_game_object.h"
#include "resources.h"

const int RADAR_X = 90;
const int RADAR_Y = 0;
const vec2 RADAR_CENTRE(RADAR_X + 8, RADAR_Y + 8);

hud::hud()
{
}

bool hud::load()
{
  std::shared_ptr<config_file> config = resources().get<config_file>("config.txt");
  float life_cell_time = config->get_float("hud::life_cell_time");

  m_font.load(get_data_dir() + "hud.font");

  m_life_full.set_image(resources().get<image>(get_data_dir() + "life_full.png"));
  m_life_full.set_num_cells(4, 1);
  m_life_full.set_cell_range(0, 3);
  m_life_full.set_cell_time(life_cell_time);
  m_life_full.set_bounce_mode(true);

  m_radar = resources().get<image>(get_data_dir() + "radar.png");
  m_blips.set_image(resources().get<image>(get_data_dir() + "blips.png"));
  m_blips.set_num_cells(3, 2);

  return true;
}

void hud::update(float dt)
{
  m_life_full.update(dt);
}

nice_font& hud::get_font()
{
  return m_font;
}

void hud::draw_blip(jammy_game_object* h, int cell)
{
  vec2 d = h->get_pos() - the_level_manager.get_level().get_player().get_pos();
  const float MAX_DIST = 200.f;
  float dist_sq = squared_length(d);
  if (dist_sq < MAX_DIST * MAX_DIST)
  {
    vec2 pos = RADAR_CENTRE + d * (10.f / MAX_DIST);
    m_blips.draw_cell<jb_mask>(the_screen, cell, pos.x, pos.y);
  }
}

void hud::draw_radar()
{
  // Draw radar
  blit<jb_mask>(m_radar, the_screen, RADAR_X, RADAR_Y);

  // TODO for all game objects, call draw_radar_blip()
}

void hud::draw_lives()
{
  int lives = the_player_progress.get_lives();
  for (int i = 0; i < lives; i++)
  {
    const int HEART_W = 10;
    m_life_full.draw<jb_mask>(the_screen, PRETEND_SCREEN_W - (i + 1) * HEART_W, 2);
  }
}

void hud::draw()
{
  draw_lives();
  draw_radar();

  // Draw score
  get_font().draw<jb_font_mask>(the_screen, 1, 1, std::to_string(the_player_progress.get_score()));
}

