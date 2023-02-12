#pragma once

#include "font.h"
#include "image.h"
#include "image_colour_xform.h"
#include "image_scale.h"

//enum just
//{
//  left, right, centre
//};

class nice_font
{
public:
  bool load(const std::string& filename);

//  nice_font& set_region(...);
  nice_font& set_scale(float scale);
  nice_font& set_colour(const colour& colour);

  template<class BLENDER>
  nice_font& draw(ref_image dest, int x, int y, const std::string& text)
  {
    m_font.draw<BLENDER>(dest, x, y, text);
    return *this;
  }
 
private:
  p_image m_image;
  std::shared_ptr<image_scale> m_scale;
  std::shared_ptr<image_colour_xform> m_colour;
  font m_font; 
};

using p_font = std::shared_ptr<nice_font>;
