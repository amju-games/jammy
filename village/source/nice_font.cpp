#include "config_file.h"
#include "directory.h"
#include "jammy_blend.h"
#include "nice_font.h"
#include "resources.h"

bool nice_font::load(const std::string& filename)
{
  text_config_file cf;
  if (!cf.load(filename))
  {
    return false;
  }
  std::string font_image = cf.get_string("font_image");
  m_image = resources().get<image>(font_image);

  m_scale = std::make_shared<image_scale>(m_image);

  m_colour = std::make_shared<image_colour_xform>(m_scale);

  m_font.set_image(m_colour);
  m_font.set_num_cells(16, 4); // TODO load info from file

  return true;
}

nice_font& nice_font::set_scale(float scale)
{
  m_scale->set_scale(scale);
  m_font.recalc_cell_size();
  return *this;
}

nice_font& nice_font::set_colour(const colour& colour)
{
  f_colour f(colour);
  m_colour->set_mult(f);
  return *this;
}


