#include <cassert>
#include <fstream>
#include <iostream>
#include "directory.h"
#include "globals.h"
#include "jammy_blend.h"
#include "human_list.h"
#include "resources.h"

void human_list::draw_human_bio(int h)
{
  assert(h < m_humans.size());

  m_ss.draw_cell<jb_overwrite>(the_screen, h, 1, 100);
  the_font.draw<jb_font_mask>(the_screen, 13, 100, m_humans[h]->m_name);
  the_font.draw<jb_font_mask>(the_screen, 13, 106, m_humans[h]->m_text);
}

bool human_list::load()
{
  // Load sprite sheet
  p_image im = resources().get<image>(get_data_dir() + "Humans/humans_indexed.png");
  if (!im)
  {
    std::cout << "FAILED TO LOAD HUMANS\n";
    assert(0);
    return false;
  }
  m_ss.set_image(im);
  m_ss.set_num_cells(23, 1);

  std::ifstream f;
  f.open(get_data_dir() + "Humans/humans.txt");
  if (!f.is_open())
  {
    return false;
  }

  std::string s;
  while (true)
  {
    std::getline(f, s);

    std::cout << "Line: '" << s << "'\n";

    if (s == "end")
    {
      return true;
    }

    human* h = new human;
    
    h->m_name = s;

    std::getline(f, s);
    std::cout << "Line: '" << s << "'\n";
    h->m_text = s;
  
    m_humans.push_back(h);
    std::cout << "Loaded " << h->m_name << "\n";

  }

  return true;
}


