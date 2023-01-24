#pragma once

#include <memory>
#include <vector>
#include "jammy_game_object.h"

class rock : public jammy_game_object
{
public:
  rock(int size);

  void explode();
  void add_child(std::shared_ptr<rock>& child);

private:
  std::vector<std::shared_ptr<rock>> m_children;
};

