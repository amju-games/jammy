# pragma once

#include "game_state.h"
#include "nice_font.h"

class jammy_game_state : public game_state
{
public:
  jammy_game_state();

  void on_active() override;

  void draw() override;
  void update(float dt) override;

  bool on_keyboard_action(const keyboard_action&) override; 

  float get_time_in_state() const { return m_time_in_state; }

  nice_font& get_font();

private:
  float m_time_in_state = 0;

  // On the assumption that every game state could use a font.
  // Not a resource, although the underlying image is a resource.
  // This lets us change font properties like size and colour without
  //  affecting anything else.
  nice_font m_font;
  std::string m_font_filename;
};

