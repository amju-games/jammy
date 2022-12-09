#pragma once

#include "jammy_game_object.h"

class player;

class human : public jammy_game_object
{
public:
  human();

  void update(float dt) override;
  void draw(screen& dest) override;

  void set_rescued(bool rescued) { m_rescued = rescued; }

  bool is_rescued() const { return m_rescued; }

  // The character we should follow
  void set_parent(jammy_game_object* p) { m_parent = p; }

private:
  jammy_game_object* m_parent = nullptr;
  bool m_rescued = false;
};

