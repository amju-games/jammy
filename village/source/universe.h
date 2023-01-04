#pragma once

#include "vec2.h"

// Universe stretches from -UNIVERSE_SIZE/2 to UNIVERSE_SIZE/2 in x and y
extern int UNIVERSE_SIZE; 

// Coords of player in screen space, i.e. at the centre of the screen.
extern vec2 CENTRE_SCREEN; 

// Size of window in actual device pixels
extern int WINDOW_W;
extern int WINDOW_H;

// Number of pixels resolution in x and y
extern int PRETEND_SCREEN_W;
extern int PRETEND_SCREEN_H;
