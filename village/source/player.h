#pragma once

#include "walker.h"

class player : public walker
{
public:
  player();

  void update(float dt) override;

  void draw(ref_image dest) override;

  // * move *
  // Move in the given direction, a bitfield combination of 
  //  directions.
  void move(int move_dir);

  // Shoot in direction of movement
  void shoot();

  int lose_life();

  void keep_immune();
 
  bool is_immune() const { return m_is_immune; }

  int get_num_lives() const { return m_lives; }

  int get_score() const { return m_score; }
 
  void add_score(int add) { m_score += add; }

  void add_human_saved() { m_humans_saved++; }

  int get_num_humans_saved() const { return m_humans_saved; }

  const vec2& get_last_move_dir() const { return m_last_move_dir; }

  // For Defender-style firing, we need to know if we're facing left or right
  // TODO This could go in Walker perhaps?
  enum class player_dir { LEFT, RIGHT };
  player_dir get_player_dir() const { return m_player_dir; }

private:
  bool m_is_immune = false;
  float m_immune_time = 0;
  int m_lives = 0;
  int m_score = 0;
  int m_humans_saved = 0;
  vec2 m_last_move_dir; // TODO deprecated? is still useful?
  player_dir m_player_dir = player_dir::RIGHT;

  sprite m_flames;
  bool m_flame_on = false;
};

