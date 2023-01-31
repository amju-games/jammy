#pragma once

#include "circular_buffer.h"
#include "jammy_collision_mgr.h"
#include "player_bullet.h"
#include "rock.h"

class level
{
public:
  bool is_level_completed() const;

  void col_det();

  // Used to procedurally create levels
  void set_level_num(int level_num) { m_level_num = level_num; }
  
  // Load part of a level: return percentage of level loaded
  //int load_part_of_level();

  void load();

  void dec_num_rocks();

  void add_player_bullet();

  player& get_player();
 
private:
  std::shared_ptr<rock> add_rock_and_descendants(int size, int child_index);

protected:
  int m_level_num = 0;

  std::shared_ptr<player> m_player;

  std::unique_ptr<circular_buffer<player_bullet>> m_player_bullets;

  // This is the number of rocks still alive in the level - when it hits zero,
  //  the level is complete (if we only care about rocks)
  int m_num_rocks_in_level = 0;

  jammy_collision_mgr m_collision_mgr;
};


