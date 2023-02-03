#include <cassert>
#include "file.h"
#include "directory.h"
#include "hi_scores.h"

bool hi_scores::load(const std::string& filename)
{
  if (!file_exists(filename))
  {
    return false;
  }
  text_file f;
  if (!f.open_for_reading(filename))
  {
    return false;
  }

  int num_scores = 0;
  if (!f.read_int(num_scores))
  {
    return false;
  }
  for (int i = 0; i < num_scores; i++)
  {
    // Read score/name pairs
    int score = 0;
    std::string name;
    if (!f.read_int(score))
    {
      return false;
    }
    if (!f.read_string(name))
    {
      return false;
    }
    m_map.insert(std::make_pair(score, name));
  }

  return true;
}

bool hi_scores::save(const std::string& filename)
{
  text_file f;
  if (!f.open_for_writing(filename))
  {
    return false;
  }
  if (!f.write_int(m_map.size()))
  {
    return false;
  }
  for (auto [score, name] : m_map)
  {
    if (!f.write_int(score) || !f.write_string(name))
    {
      return false;
    }
  }
  return true;
}

bool hi_scores::is_hi_score(int score)
{
  if (m_map.empty())
  {
    return true;
  }

  if (m_map.size() < m_max_num_hi_scores)
  {
    return true;
  }

  // Greater than lowest hi score?
  return (score >= m_map.begin()->first);
}

void hi_scores::store(int score, const std::string& name)
{
  if (!is_hi_score(score))
  {
    return;
  }

  while (m_map.size() >= m_max_num_hi_scores)
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

