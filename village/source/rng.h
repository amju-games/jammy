#pragma once

#include <random>

class rng
{
public:
  rng();

  // Return random float in range [0, n)
  float rand(float n);

  // Return random float in range [m, n)
  float rand(float m, float n);

  // Return random int in range [0, n)
  int rand(int n);

  // Return random int in range [m, n)
  int rand(int m, int n);

private:
  std::mt19937 m_generator;
};
