#include <iostream>

#include "jammy_game_state.h"
#include "resources.h"

void jammy_game_state::on_keyboard_action(const keyboard_action& ka)
{
  // Hard coded debug commands
  auto [key, state](ka);
  switch (key)
  {
  case 'R':
    [[fallthrough]]
    ;
  case 'r':
std::cout << "HOT RELOAD!\n";
    resources().reload();
    break;
  };
} 

