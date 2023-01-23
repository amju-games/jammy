#pragma once

#include <vector>
#include "circular_buffer.h"
#include "jammy_collision_mgr.h"
#include "jammy_game_state.h"
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

  player* get_player() { return m_player; }

protected:
  void add_player_bullet();
  void col_det();
  void draw_blip(jammy_game_object* h, int cell);

protected:
  player* m_player = nullptr;
  hq* m_hq = nullptr;

  std::unique_ptr<circular_buffer<player_bullet>> m_player_bullets;
  std::vector<human*> m_humans;
  std::vector<human*> m_rescued_humans;

  std::vector<rock*> m_rocks;

  p_image m_radar;
  sprite_sheet m_blips;
  p_image m_life_empty;
  p_image m_life_full;

  jammy_collision_mgr m_collision_mgr;
};

