#include <iostream>

#include "config_file.h"
#include "draw_ellipse.h"
#include "image_32.h"
#include "moon.h"
#include "resources.h"

moon::moon()
{
//  auto config = resources().get<config_file>("config.txt");

  sprite s;

  p_image outer = std::make_shared<image_32>();
  outer->set_size(64, 64);
  outer->clear(colour(0, 0, 0, 0));
  draw_ellipse_solid(outer, 32, 32, 30, 30, colour(0xff, 0xc0, 0x00));

  s.set_image(outer);
 
  s.set_num_cells(1, 1);
  set_sprite(s);

  set_is_collidable(true);
}

void moon::on_shot_by_player(player_bullet* )
{
  // Work out collision point. Subtract a sphere at that point from the outer sprite.

  // Convert contact point to local coords.
  // Convert from top left to centre of subtracted sphere
  vec2 contact_point(8, 20);

  p_image subtract = std::make_shared<image_32>();
  int s = 20; // size of hole we subtract
  subtract->set_size(s, s);
  subtract->clear(colour(0, 0, 0, 0));
  draw_ellipse_solid(subtract, s / 2, s / 2, s / 2, s / 2, colour(0xff, 0xff, 0xff, 0xff));

  p_image im = get_sprite().get_image();

  blit<sub_blend>(subtract, im, contact_point.x, contact_point.y);

}

