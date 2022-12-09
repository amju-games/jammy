#include <cassert>
#include "hi_scores.h"


bool hi_scores::load()
{
  return false;
}

bool hi_scores::save()
{
  return false;
}

bool hi_scores::is_hi_score(int score)
{
  if (m_map.empty())
  {
    return true;
  }
  // Greater than lowest hi score?
  return (score > m_map.begin()->first);
}

void hi_scores::store(int score, const std::string& name)
{
  if (!is_hi_score(score))
  {
    return;
  }

  while (m_map.size() >= MAX_NUM_HI_SCORES)
  {
    m_map.erase(m_map.begin()); // remove lowest score
  }
  m_map.insert(std::make_pair(score, name));
}

int hi_scores::get_num_hi_scores() const
{
  return m_map.size();
}

void hi_scores::get_hi_score(int n, int& score, std::string& name)
{
  assert(n < m_map.size());
  auto it = m_map.begin();
  std::advance(it, n);
  score = it->first;
  name = it->second;
}

