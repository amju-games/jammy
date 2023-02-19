#include "rng.h"

rng::rng() : m_generator(std::random_device()())
{
}

float rng::rand(float n)
{
  std::uniform_real_distribution<> dist(0, n);
  return dist(m_generator);
}

float rng::rand(float m, float n)
{
  std::uniform_real_distribution<> dist(m, n);
  return dist(m_generator);
}

int rng::rand(int n)
{
  std::uniform_int_distribution<> dist(0, n - 1);
  return dist(m_generator);
}

int rng::rand(int m, int n)
{
  std::uniform_int_distribution<> dist(m, n - 1);
  return dist(m_generator);
}

