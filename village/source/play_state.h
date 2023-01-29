#pragma once

#include <vector>
#include "circular_buffer.h"
#include "jammy_collision_mgr.h"
#include "jammy_game_state.h"
#include "player.h"
#include "player_bullet.h"

class hq;
class human;
class jammy_game_object;
class player;
class player_bullet;
class rock;

class play_state : public jammy_game_state
{
public:
  play_state();
  void update(float dt) override;
  void draw() override;
  void on_input(int input) override;
  void on_active() override;
  void on_deactive() override;

  void on_dir_button_action(const dir_button_action&) override;

  std::shared_ptr<player> get_player() { return m_player; }

  // TODO Should be in a level class
  void dec_num_rocks();

protected:
  void add_player_bullet();

  // Recursively add rocks
  std::shared_ptr<rock> add_rock_and_descendants(int level, int child_index);

  void col_det();
  void draw_radar();
  void draw_lives();
  void draw_blip(jammy_game_object* h, int cell);

protected:
  std::shared_ptr<player> m_player;
  hq* m_hq = nullptr;

  std::unique_ptr<circular_buffer<player_bullet>> m_player_bullets;
  std::vector<human*> m_humans;
  std::vector<human*> m_rescued_humans;

  std::vector<std::shared_ptr<rock>> m_rocks;
  // This is the number of rocks still alive in the level - when it hits zero,
  //  the level is complete (if we only care about rocks)
  int m_num_rocks_in_level = 0;

  p_image m_radar;
  sprite_sheet m_blips;
  p_image m_life_empty;
  p_image m_life_full;

  jammy_collision_mgr m_collision_mgr;
};

