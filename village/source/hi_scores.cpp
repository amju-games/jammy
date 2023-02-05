#include <iostream>

#include <cassert>
#include "file.h"
#include "directory.h"
#include "hi_scores.h"

std::string hi_scores_filename()
{
  std::string res = get_save_dir() + "/hi_scores";
std::cout << "Hi scores filename: " << res << "\n";
  return res;
}

bool hi_scores::load(const std::string& filename)
{
  if (!file_exists(filename))
  {
std::cout << "Hi scores: no file " << filename << "\n";
    return false;
  }
  text_file f;
  if (!f.open_for_reading(filename))
  {
std::cout << "Hi scores: file " << filename << " exists, but failed to open for reading\n";
    return false;
  }

  int num_scores = 0;
  if (!f.read_int(num_scores))
  {
std::cout << "Hi scores: file " << filename << " exists, but failed to read number of hi scores\n";
    return false;
  }
  for (int i = 0; i < num_scores; i++)
  {
    // Read score/name pairs
    int score = 0;
    std::string name;
    if (!f.read_int(score))
    {
std::cout << "Hi scores: file " << filename << " exists, but failed to read score " << i << "\n";
      return false;
    }
    if (!f.read_string(name))
    {
std::cout << "Hi scores: file " << filename << " exists, but failed to read name " << i << "\n";
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
std::cout << "Hi scores: failed to open " << filename << " for writing\n";
    return false;
  }
  if (!f.write_int(m_map.size()))
  {
std::cout << "Hi scores: opened " << filename << " for writing but failed to write num hi scores\n";
    return false;
  }
  for (auto [score, name] : m_map)
  {
    if (!f.write_int(score) || !f.write_string(name))
    {
std::cout << "Hi scores: opened " << filename << " for writing but failed to write\n";
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

