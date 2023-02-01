#pragma once

#include "image.h"
#include "sprite_sheet.h"

class jammy_game_object;

class hud
{
public:
  hud();
  void draw();

protected:
  void draw_radar();
  void draw_lives();
  void draw_blip(jammy_game_object* h, int cell);

protected:
  p_image m_radar;
  sprite_sheet m_blips;
  p_image m_life_empty;
  p_image m_life_full;

};

