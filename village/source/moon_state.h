#pragma once

#include "play_state_base.h"

class moon_state : public play_state_base
{
public:
  void on_active() override;
};

