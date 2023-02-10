#include "level_manager.h"

level& level_manager::get_level()
{
  return *m_level;
}

void level_manager::make_new_level()
{
  m_level = std::make_unique<level>();
}


