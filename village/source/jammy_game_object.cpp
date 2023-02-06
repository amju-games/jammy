//#define COUNT_GAME_OBJECTS

#ifdef COUNT_GAME_OBJECTS
#include <iostream>
#endif

#include "jammy_blend.h"
#include "jammy_game_object.h"
#include "string_utils.h"
#include "universe.h"

vec2 jammy_game_object::s_cam_pos;

static int num_game_objects = 0;

jammy_game_object::jammy_game_object()
{
  num_game_objects++;

#ifdef COUNT_GAME_OBJECTS
std::cout << "Num game objects: " << num_game_objects << "\n";
#endif
}

jammy_game_object::~jammy_game_object()
{
  num_game_objects--;

#ifdef COUNT_GAME_OBJECTS
std::cout << "Num game objects: " << num_game_objects << "\n";
#endif
}

bool sprite_collision(
  jammy_game_object* jgo1, jammy_game_object* jgo2)
{
  pix_int_result r = pixel_intersect<jb_transparent>(
    jgo1->m_sprite, jgo1->m_sprite.get_cell(), jgo1->m_pos.x, jgo1->m_pos.y,
    jgo2->m_sprite, jgo2->m_sprite.get_cell(), jgo2->m_pos.x, jgo2->m_pos.y);

  return (r == pix_int_result::YES_COLLIDE);
}

std::string jammy_game_object::print() const
{
  return concat(typeid(*this).name(), ": ", get_id()); 
}

void jammy_game_object::set_cam_pos(const vec2& pos)
{
  s_cam_pos = pos;
}

void jammy_game_object::draw(ref_image dest)
{
  if (!is_alive() || !is_visible())
  {
    return;
  }

  draw_sprite<jb_mask>(m_sprite, dest);
}

void jammy_game_object::update(float dt) 
{
  if (!is_alive())
  {
    return;
  }

  game_object::update(dt);

  // Wrap around
  vec2 rel_pos = m_pos - s_cam_pos;

  if (rel_pos.x < -(UNIVERSE_SIZE / 2 + m_sprite.get_cell_w()))
  {
    m_pos.x += UNIVERSE_SIZE + m_sprite.get_cell_w();
  }
  if (rel_pos.x > (UNIVERSE_SIZE / 2))
  {
    m_pos.x -= (UNIVERSE_SIZE + m_sprite.get_cell_w());
  }
  if (rel_pos.y < -(UNIVERSE_SIZE / 2 + m_sprite.get_cell_h()))
  {
    m_pos.y += UNIVERSE_SIZE + m_sprite.get_cell_h();
  }
  if (rel_pos.y > (UNIVERSE_SIZE / 2))
  {
    m_pos.y -= (UNIVERSE_SIZE + m_sprite.get_cell_h());
  }

  m_sprite.update(dt);
}

