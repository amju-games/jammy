#include "level_manager.h"

level& level_manager::get_level()
{
  static level lev;
  return lev;
}

