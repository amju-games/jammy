#pragma once

#include <string>
#include "file.h"
#include "sprite_sheet.h"

class human_list
{
public:
  struct human
  {
    std::string m_name;
    std::string m_text;
    image m_image;
  };

  void draw_human_bio(int human_to_display);

  bool load();

private:
  sprite_sheet m_ss;
  std::vector<human*> m_humans;
};

