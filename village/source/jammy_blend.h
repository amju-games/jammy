#pragma once

#include "blend.h"
#include "colour.h"

// Blend functor for blitting: magenta is treated as transparent, and any colour with
//  alpha of zero.

struct jb_transparent
{
  bool operator()(const colour& c)
  {
//    return (c.a == 0);

    return (c.a == 0) ||
           (c.r == 0xff && c.g == 0 && c.b == 0xff);
  }
};

using jb_overwrite = overwrite;

using jb_mask = mask<jb_transparent>;

using jb_font_mask = jb_mask;

