#pragma once

#include "level.h"

class level_manager
{
public:
  void make_new_level();

  level& get_level();

private:
  std::unique_ptr<level> m_level;
};

