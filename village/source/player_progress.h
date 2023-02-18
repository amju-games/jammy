#pragma once

class player_progress
{
public:
  bool initialise();

  int get_score() const { return m_score; }
  int get_level() const { return m_level; }
  int get_lives() const { return m_lives; }

  void add_score(int s);
  void set_level(int level) { m_level = level; }
  void dec_lives() { m_lives--; }
  void inc_lives() { m_lives++; }

private:
  int m_score = 0;
  int m_level = 1;
  int m_lives = 3;
};

