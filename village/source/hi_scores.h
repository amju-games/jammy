#pragma once

#include <map>
#include <string>

class hi_scores
{
public:
  void set_max_num_hi_scores(int max) { m_max_num_hi_scores = max; }

  bool load(const std::string& filename);
  bool save(const std::string& filename);

  bool is_hi_score(int score);
  void store(int score, const std::string& name);
  int get_num_hi_scores() const;

  // 0 <= n < get_num_hi_scores()
  void get_hi_score(int n, int& score, std::string& name);

private:
  // Store hi scores in a map to keep them in order. 
  // Ascending order
  std::multimap<int, std::string> m_map;

  int m_max_num_hi_scores = 10;
};

