#include <iostream>

#include <algorithm>
#include "config_file.h"
#include "draw_ellipse.h"
#include "image_32.h"
#include "moon.h"
#include "player_bullet.h"
#include "resources.h"

moon::moon()
{
  auto config = resources().get<config_file>("config.txt");
  int size = config->get_int("moon::size");

  sprite s;

  p_image outer = std::make_shared<image_32>();
  outer->set_size(size, size);
  outer->clear(colour(0, 0, 0, 0));
  draw_ellipse_solid(outer, size / 2, size / 2, size / 2 - 2, size / 2 - 2, 
    colour(0xff, 0xc0, 0x00)); // TODO

  s.set_image(outer);
 
  s.set_num_cells(1, 1);
  set_sprite(s);

  set_is_collidable(true);
}

vec2 moon::get_rel_pos(player_bullet* pb)
{
  vec2 rel_pos = pb->get_pos() - get_pos();
  assert(rel_pos.y >= 0);
  assert(rel_pos.y < get_sprite().get_image()->get_height());
  return rel_pos;
}

vec2 moon::find_contact_point_from_right(player_bullet* pb)
{
  p_image im = get_sprite().get_image();

  // Find point on this corresponding to rightmost (x, y) of pb,
  //  then move left until we find a non-transparent pixel in this.
  vec2 rel_pos = get_rel_pos(pb);
  const int w = im->get_width();
  int min_x = std::max(0, static_cast<int>(rel_pos.x));
  int max_x = std::min(w - 1, static_cast<int>(rel_pos.x) + pb->get_sprite().get_image()->get_width());
  assert(min_x >= 0);
  assert(max_x < w);
  for (int x = max_x; x >= min_x; x--)
  {
    const colour& c = im->get_colour(x, rel_pos.y);
    if (c.a != 0)
    {
      return vec2(x, rel_pos.y);
    }
  }
  assert(false);
  return vec2(min_x, rel_pos.y);
}

vec2 moon::find_contact_point_from_left(player_bullet* pb)
{
  p_image im = get_sprite().get_image();

  // Find point on this corresponding to leftmost (x, y) of pb,
  //  then move right until we find a non-transparent pixel in this.
  vec2 rel_pos = get_rel_pos(pb);
  const int w = im->get_width();
  int min_x = std::max(0, static_cast<int>(rel_pos.x));
  int max_x = std::min(w - 1, w + static_cast<int>(rel_pos.x));
  assert(min_x >= 0);
  assert(max_x < w);
  for (int x = min_x; x < max_x; x++)
  {
    const colour& c = im->get_colour(x, rel_pos.y);
    if (c.a != 0)
    {
      return vec2(x, rel_pos.y);
    }
  }
  assert(false);
  return vec2(max_x, rel_pos.y);
}


vec2 moon::find_contact_point(player_bullet* pb)
{
  // Search along pb for the first non-transparent pixel in this.
  // If pb is travelling right, search from left, and vice versa.
  if (pb->get_vel().x > 0)
  {
    return find_contact_point_from_left(pb);
  } 
  else
  {
    return find_contact_point_from_right(pb);
  }
}

void moon::on_shot_by_player(player_bullet* pb)
{
  // Work out collision point. Subtract a sphere at that point from the outer sprite.
  vec2 contact_point = find_contact_point(pb); 

  auto config = resources().get<config_file>("config.txt");
  int s = config->get_int("moon::hole::size");

  p_image subtract = std::make_shared<image_32>();
  subtract->set_size(s, s);
  subtract->clear(colour(0, 0, 0, 0));
  draw_ellipse_solid(subtract, s / 2, s / 2, s / 2, s / 2, colour(0xff, 0xff, 0xff, 0xff));

  contact_point -= vec2(s / 2, s / 2); // centre hole on contact point

  p_image im = get_sprite().get_image();

  blit<sub_blend>(subtract, im, contact_point.x, contact_point.y);

}

