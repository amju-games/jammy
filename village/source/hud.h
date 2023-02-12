#pragma once

#include "image.h"
#include "nice_font.h"
#include "sprite_sheet.h"

class jammy_game_object;

class hud
{
public:
  hud();
  bool load();
  void draw();

protected:
  p_font& get_font();
  void draw_radar();
  void draw_lives();
  void draw_blip(jammy_game_object* h, int cell);

protected:
  p_image m_radar;
  sprite_sheet m_blips;
  p_image m_life_empty;
  p_image m_life_full;
  p_font m_font;
};

