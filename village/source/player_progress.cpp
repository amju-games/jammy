#include "config_file.h"
#include "player_progress.h"
#include "resources.h"

bool player_progress::initialise()
{
  auto config = resources().get<config_file>("config.txt");
  m_lives = config->get_int("player_progress::lives");
  m_level = config->get_int("player_progress::level");
  m_score = 0;

  return true;
}

void player_progress::add_score(int s)
{
  if (m_lives > 0) // ignore updates if game is over
  {
    m_score += s;
  }
}

