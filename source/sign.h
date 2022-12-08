#pragma once

inline int sign(float f)
{
  if (f < 0)
  {
    return -1;
  }
  else if (f > 0)
  {
    return 1;
  }
  return 0;
}

