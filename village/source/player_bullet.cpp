#include "directory.h"
#include "globals.h"
#include "image_colour_xform.h"
#include "player_bullet.h"
#include "resources.h"
#include "universe.h"

player_bullet::player_bullet() 
{
  p_image im = resources().get<image>(get_data_dir() + "player_bullet_2.png");
  m_colour_decorator = std::make_shared<image_colour_xform>();
  m_colour_decorator->set_child(im);
  m_sprite.set_image(m_colour_decorator);
  m_sprite.set_num_cells(1, 32);
  m_sprite.set_cell_time(0.05f);
}

void player_bullet::fire(std::shared_ptr<player>& player)
{
  set_is_alive(true);
  set_is_collidable(true);

  auto dir = player->get_player_dir();
  const float BULLET_SPEED = 150.0f;

  vec2 offset(0, 7); // TODO depends on gun type
  vec2 bullet_vel;
  if (dir == player::player_dir::LEFT)
  {
    m_sprite.set_cell_range(16, 31);
    m_sprite.set_cell(16); // so reused bullet is reset to start of cell range
    offset.x = -56;
    bullet_vel = vec2(-BULLET_SPEED, 0.f);
  }
  else
  {
    m_sprite.set_cell_range(0, 15);
    m_sprite.set_cell(0); 
    offset.x = 16;
    bullet_vel = vec2( BULLET_SPEED, 0.f);
  }
  set_pos(offset + player->get_pos()); 

  set_vel(bullet_vel);


  // Set colour on sprite.
  static int index = 0;
  const std::array<alg3::vec4, 6> COLOURS = 
  {
    alg3::vec4{ 1, 1, 1, 1 },
    alg3::vec4{ 1, 1, 0, 1 },
    alg3::vec4{ 1, 0, 1, 1 },
    alg3::vec4{ 0, 1, 1, 1 },
    alg3::vec4{ 1, 0, 0, 1 },
    alg3::vec4{ 0, 1, 0, 1 }
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
  if (!is_alive())
  {
    return;
  }

  jammy_game_object::update(dt);

  // Die if we are a long way from player (just need to check x, bullets only go horiz)
  float dist = m_pos.x - s_cam_pos.x;
  if (fabs(dist) > PRETEND_SCREEN_W)
  {
    set_is_alive(false);
    set_is_collidable(false);
  }
}


