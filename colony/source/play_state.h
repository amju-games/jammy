#pragma once

#include <vector>
#include "jammy_game_state.h"

class hq;
class human;
class jammy_game_object;
class player;
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
  void col_det();
  void draw_blip(jammy_game_object* h, int cell);

protected:
  player* m_player = nullptr;
  hq* m_hq = nullptr;

  std::vector<human*> m_humans;
  std::vector<human*> m_rescued_humans;

  std::vector<rock*> m_rocks;

  image m_radar;
  sprite_sheet m_blips;
  image m_life_empty;
  image m_life_full;
};

