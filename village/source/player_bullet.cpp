#include "directory.h"
#include "globals.h"
#include "image_colour_xform.h"
#include "player_bullet.h"
#include "resources.h"
#include "universe.h"

player_bullet::player_bullet() 
{
  p_image im = resources().get<image>("player_bullet_2.png");
  m_colour_decorator = std::make_shared<image_colour_xform>();
  m_colour_decorator->set_child(im);
  sprite(s);
  s.set_image(m_colour_decorator);
  s.set_num_cells(1, 32);
  s.set_cell_time(0.05f);
  set_sprite(s);
  set_is_updatable(false);
  set_is_visible(false);
  set_is_collidable(false);
}

void player_bullet::fire(std::shared_ptr<player>& player)
{
  set_is_updatable(true);
  set_is_collidable(true);
  set_is_visible(true);

  auto dir = player->get_player_dir();
  const float BULLET_SPEED = 150.0f;

  vec2 offset(0, 7); // TODO depends on gun type
  vec2 bullet_vel;
  if (dir == player::player_dir::LEFT)
  {
    get_sprite().set_cell_range(16, 31);
    get_sprite().set_cell(16); // so reused bullet is reset to start of cell range
    offset.x = -56;
    bullet_vel = vec2(-BULLET_SPEED, 0.f);
  }
  else
  {
    get_sprite().set_cell_range(0, 15);
    get_sprite().set_cell(0); 
    offset.x = 16;
    bullet_vel = vec2( BULLET_SPEED, 0.f);
  }
  set_pos(offset + player->get_pos()); 

  set_vel(bullet_vel);


  // Set colour on sprite.
  static int index = 0;
  const std::array<f_colour, 6> COLOURS = 
  {
    f_colour{ 1, 1, 1, 1 },
    f_colour{ 1, 1, 0, 1 },
    f_colour{ 1, .1f, 1, 1 }, // yuck, pure magenta is treated as transparent here.
    f_colour{ 0, 1, 1, 1 },
    f_colour{ 1, 0, 0, 1 },
    f_colour{ 0, 1, 0, 1 }
  };  
  m_colour_decorator->set_mult(COLOURS[index]);
  index++;
  if (index == COLOURS.size())
  {
    index = 0;
  }
}

void player_bullet::update(float dt)
{
  if (!is_updatable())
  {
    return;
  }

  jammy_game_object::update(dt);

  // Die if we are a long way from player (just need to check x, bullets only go horiz)
  float dist = m_pos.x - get_cam_pos().x;
  if (fabs(dist) > PRETEND_SCREEN_W)
  {
    set_is_updatable(false);
    set_is_collidable(false);
    set_is_visible(false);
  }
}


