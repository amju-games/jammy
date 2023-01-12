#pragma once

#include "multi_manager.h"

// Create/load resources through the object returned by this function.
// E.g. resources().get<image>("image.png");
multi_manager& resources();


