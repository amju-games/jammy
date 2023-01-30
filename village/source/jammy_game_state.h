# pragma once

#include "game_state.h"

class jammy_game_state : public game_state
{
public:
  void on_keyboard_action(const keyboard_action&) override; 

protected:
};

