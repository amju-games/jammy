#pragma once

#ifdef CIRC_BUFF_DEBUG
#include <iostream>
#endif

#include <memory>
#include <vector>
#include "game.h"

// Circular buffer, for handling a pool of game objects like bullets.

template<class T>
class circular_buffer
{
public:
  circular_buffer(game& the_game, int max_buffer_size) : m_game(the_game), m_max_buffer_size(max_buffer_size) {}

  std::shared_ptr<T> get_next_element()
  {
    if (m_buffer.size() < m_max_buffer_size)
    {
#ifdef CIRC_BUFF_DEBUG
std::cout << "Making a new " << typeid(T).name() << "\n";
#endif

      auto t = std::make_shared<T>();
      m_buffer.push_back(t);
      m_game.add_game_object(t);
      return t;
    }
    else
    {
#ifdef CIRC_BUFF_DEBUG
std::cout << "Reusing old" << typeid(T).name() << " " << m_index << "\n";
#endif

      auto t = m_buffer[m_index];

      ++m_index;
      if (m_index >= m_buffer.size())
      {
        m_index = 0;
      }

      return t;
    }
  }

private:
  game& m_game;
  int m_max_buffer_size = 1;
  int m_index = 0;
  std::vector<std::shared_ptr<T>> m_buffer;
};

