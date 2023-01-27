#pragma once

#include <memory>
#include <vector>
#include "jammy_game_object.h"

class rock : public jammy_game_object
{
public:
  rock(int size_level, int child_index);

  void draw(ref_image dest) override;
  void update(float dt) override;

  void explode();
  void add_child(std::shared_ptr<rock>& child);

  void set_is_exploding(bool exploding) { m_is_exploding = exploding; }

  int get_score() const;

private:
  std::vector<std::shared_ptr<rock>> m_children;

  sprite m_explosion; 
  bool m_is_exploding = false;
  int m_size_level = 0; // biggest size is 0, next is 1, smallest is 2
};

