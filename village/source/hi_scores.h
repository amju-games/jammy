#pragma once

#include <map>
#include <string>

class hi_scores
{
public:
  static const int MAX_NUM_HI_SCORES = 10;

  bool load();
  bool save();

  bool is_hi_score(int score);
  void store(int score, const std::string& name);
  int get_num_hi_scores() const;

  // 0 <= n < get_num_hi_scores()
  void get_hi_score(int n, int& score, std::string& name);

private:
  // Store hi scores in a map to keep them in order. 
  // Ascending order
  std::multimap<int, std::string> m_map;
};

