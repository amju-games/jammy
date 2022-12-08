# pragma once

#include "game_state.h"

class game;
class screen;

class jammy_game_state : public game_state
{
public:
  void set_game(game* g) { m_game = g; }
  void set_screen(screen* s) { m_screen = s; }

  virtual void on_input(int input) {} 

protected:
  game* m_game = nullptr;
  screen* m_screen = nullptr;
};

