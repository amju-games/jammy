#pragma once

#include "hud.h"
#include "jammy_game_state.h"

// Base class for all states displaying game objects
class play_state_base : public jammy_game_state
{
public:
  void update(float dt) override;
  void draw() override;
  void on_active() override;

protected:
  virtual void draw_game_objects();
  virtual void draw_hud();

private:
  hud m_hud;
};

