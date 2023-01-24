#include <iostream>
#include "directory.h"
#include "input.h"
#include "globals.h"
#include "player.h"
#include "sign.h"
#include "resources.h"
#include "universe.h"

//#define NO_DECEL

const float MAX_IMMUNE_TIME = 3.f;
const float DECELERATION = 0.5f;
const float FLAME_FRAME_TIME = 0.15f;
float PLAYER_SPEED = 30.f; 

player::player()
{
  m_sprite.set_image(resources().get<image>(get_data_dir() + "player1.png"));
  m_sprite.set_num_cells(3, 2);

  m_flames.set_image(resources().get<image>(get_data_dir() + "flames.png"));
  m_flames.set_num_cells(4, 6); // 4 frames * 6 directions
  m_flames.set_cell_time(FLAME_FRAME_TIME);

  m_pos = vec2(0, 0); //CENTRE_SCREEN;

  m_is_immune = false;
  m_immune_time = 0;
  m_lives = 5;
  m_last_move_dir = vec2(1.f, 0.f);
}

void player::keep_immune()
{
  m_immune_time = MAX_IMMUNE_TIME;
  m_is_immune = true;
}

int player::lose_life()
{
  if (m_lives > 0)
  {
    m_lives--;
  }

  keep_immune();

  return m_lives;
}

static float flasher = 0;
static const float FLASH_PERIOD = .3f;

void player::draw(ref_image dest)
{
  if (m_is_immune)  
  {
    if (flasher > FLASH_PERIOD * .5f)
    {
      return;
    }
  }

  walker::draw(dest);

  // Draw jet pac flame
  if (m_flame_on)
  {
    draw_sprite(m_flames, dest);
  }
}

void player::update(float dt)
{
  if (m_is_immune)
  {
    flasher += dt;
    if (flasher > FLASH_PERIOD)
    {
      flasher = 0;
    }

    m_immune_time -= dt;
    if (m_immune_time <= 0)
    {
      m_immune_time = 0;
      m_is_immune = false;
    }
  }

  vec2 old_vel = m_vel;

  set_cam_pos(m_pos); // TODO some elasticity

  jammy_game_object::update(dt); // not walker

  m_flames.update(dt);
  
  // Check for deceleration to stop
  if (sign(old_vel.x) != sign(m_vel.x))
  {
    m_vel.x = 0;
  }

  if (sign(old_vel.y) != sign(m_vel.y))
  {
    m_vel.y = 0;
  }

/*
  const float STOPPED = 0.1f;
  if (fabs(m_vel.x) < STOPPED && fabs(m_vel.y) < STOPPED)
  {
    // Set sprite to stopped
    m_sprite.set_cell_range(3, 3);
  }
*/

}

  // Flame sprite sheet cell range start positions
  namespace flame
  {
    const int R = 0;
    const int L = 4;
    const int UR = 8;
    const int UL = 12;
    const int DR = 16;
    const int DL = 20;
    const int NUM_FRAMES = 4;
  };

void player::move(int move_dir)
{
/*
  if (move_dir != MOVE_NONE)
  {
    int CELL[16];  // 16 permutations but not all are valid
    // Map move bitfield to cell position
    for (int i = 0; i < 16; i++)
    {
      CELL[i] = 3;
    }
    CELL[MOVE_RIGHT] = 0;
    CELL[MOVE_RIGHT | MOVE_UP] = 1;
    CELL[MOVE_RIGHT | MOVE_DOWN] = 2;
    CELL[MOVE_UP] = 4;
    CELL[MOVE_DOWN] = 5;
    CELL[MOVE_LEFT] = 6;
    CELL[MOVE_LEFT | MOVE_UP] = 7;
    CELL[MOVE_LEFT | MOVE_DOWN] = 8;

    int c = CELL[move_dir];
    m_sprite.set_cell_range(c, c);
  }
*/

  vec2 dir;

  m_flame_on = false;

  if (move_dir & MOVE_UP)
  {
    dir.y -= PLAYER_SPEED; 
  }
  if (move_dir & MOVE_DOWN)
  {
    dir.y += PLAYER_SPEED; 
  }
  if (move_dir & MOVE_LEFT)
  {
    m_player_dir = player_dir::LEFT;
    dir.x -= PLAYER_SPEED; 
    m_sprite.set_cell_range(3, 3);
    m_flame_on = true;
    m_flames.set_cell_range(flame::L, flame::L + flame::NUM_FRAMES - 1);
  }
  if (move_dir & MOVE_RIGHT)
  {
    m_player_dir = player_dir::RIGHT;
    dir.x += PLAYER_SPEED; 
    m_sprite.set_cell_range(0, 0);
    m_flame_on = true;
    m_flames.set_cell_range(flame::R, flame::R + flame::NUM_FRAMES - 1);
  }

#ifdef NO_DECEL
  if ((move_dir & MOVE_LEFT) == 0 && (move_dir & MOVE_RIGHT) == 0)
  {
std::cout << "Not moving left or right\n";
    dir.x = 0;
  }
  if ((move_dir & MOVE_UP) == 0 && (move_dir & MOVE_DOWN) == 0)
  {
std::cout << "Not moving up or down\n";
    dir.y = 0;
  }
#endif

  m_vel += dir; 

  // Store last non-zero vel
  if (squared_length(m_vel) > 0.f)
  {
    m_last_move_dir = m_vel;
  }

  // Max PLAYER_SPEED
  float max_PLAYER_SPEED = 20.f; // TODO
  if (squared_length(m_vel) > max_PLAYER_SPEED)
  {
    m_vel = normalise(m_vel) * max_PLAYER_SPEED;
  }

#ifndef NO_DECEL
  m_acc = -m_vel * DECELERATION; 
  
#endif
}

