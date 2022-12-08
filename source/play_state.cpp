#include <iostream>
#include <cassert>
#include "directory.h"
#include "game.h"
#include "globals.h"
#include "hq.h"
#include "human.h"
#include "jammy_game_object.h"
#include "parallax_bg.h"
#include "player.h"
#include "play_state.h"
#include "rock.h"
#include "screen.h"

const int NUM_ROCKS = 30; 
const int NUM_HUMANS = 10; 

const int PICK_UP_HUMAN_SCORE = 250;
const int DELIVER_HUMAN_SCORE = 1000;

const int RADAR_X = 0;
const int RADAR_Y = 0;
const vec2 RADAR_CENTRE(RADAR_X + 8, RADAR_Y + 8);

play_state::play_state()
{
  m_life_empty.load(get_data_dir() + "life_empty.png", the_global_palette);
  m_life_full.load(get_data_dir() + "life_full.png", the_global_palette);

  m_radar.load(get_data_dir() + "radar.png", the_global_palette);
  m_blips.load(get_data_dir() + "blips.png", the_global_palette);
  m_blips.set_num_cells(3, 2);
}

void play_state::on_input(int input)
{
  assert(m_player);
  m_player->move(input);  
}

void play_state::on_active() 
{
  std::cout << "ON ACTIVE\n";

  // Add background
  the_game.add_game_object(new parallax_bg);
  
  // Add HQ
  m_hq = new hq;
  the_game.add_game_object(m_hq);

  // Add humans
  for (int i = 0; i < NUM_HUMANS; i++)
  {
    human* h = new human;
    the_game.add_game_object(h);
    m_humans.push_back(h);
  }

  // Add player
  m_player = new player;
  the_game.add_game_object(m_player);

  // Add asteroids
  for (int i = 0; i < NUM_ROCKS; i++)
  {
    rock* r = new rock;
    the_game.add_game_object(r);
    m_rocks.push_back(r);
  }
}

void play_state::on_deactive() 
{
  m_player = nullptr; // TODO leak!
  m_humans.clear();
  m_rescued_humans.clear();
  m_rocks.clear();
  the_game.clear_game_objects();
}

void play_state::col_det()
{
  if (m_player->is_immune())
  {
    return;
  }

  // Test player and humans against rocks
  for (rock* r : m_rocks)
  {
    // Check player/rock sprites
    if (!m_player->is_immune() &&
        sprite_collision(r, m_player))
    {
      m_player->lose_life();
    }

    // Check for rocks v humans
    // TODO
    for (auto it = m_rescued_humans.begin(); it != m_rescued_humans.end(); ++it)
    {
      human* h = *it;
      if (sprite_collision(r, h))
      {
        // This human is separated from its parent in the chain
        h->set_rescued(false);
        h->set_parent(nullptr);

        // Pushed in direction of asteroid travel
        h->set_vel(h->get_vel() + r->get_vel());

        // This human and all following are no longer rescued
        for (auto jt = it; jt != m_rescued_humans.end(); ++jt)
        {
          human* h = *jt;
          h->set_rescued(false);
          h->set_parent(nullptr);
        }
        m_rescued_humans.erase(it, m_rescued_humans.end());
        break;
      }
    }
  }

  // Test for humans
  for (human* h : m_humans)
  {
    if (sprite_collision(h, m_player))
    {
      // Already rescued?
      if (std::find(m_rescued_humans.begin(), m_rescued_humans.end(), h) == m_rescued_humans.end())
      {
        // Not already rescued, so add to chain
std::cout << "RESCUED!!\n";
        m_player->add_score(PICK_UP_HUMAN_SCORE);

        h->set_rescued(true);

        if (m_rescued_humans.empty())
        {
          h->set_parent(m_player);
        }
        else
        {
          h->set_parent(m_rescued_humans.back());
        }
        m_rescued_humans.push_back(h);
      }
    }
  }

  // Check for rescued humans reaching the space ship (HQ)
  // Check for collision with HQ
  for (auto it = m_rescued_humans.begin(); it != m_rescued_humans.end(); ++it)
  {
    human* h = *it;
  
    if (sprite_collision(h, m_hq))
    {
      // This human and all descendants are delivered to the ship!
      for (auto jt = it; jt != m_rescued_humans.end(); ++jt)
      {
        m_player->add_score(DELIVER_HUMAN_SCORE);
        human* h = *jt;
        h->set_rescued(false);
        h->set_parent(nullptr);
        h->set_pos(h->get_pos() + vec2(300, 300)); // respawn
      }
      m_rescued_humans.erase(it, m_rescued_humans.end());
      break;
    }
  }
}

void play_state::update(float dt)
{
  the_game.update_game_objects(dt); 

  col_det();

  static float t = 0; 
  static float old_t = t;
  t += dt;
  if ((int)t != (int)old_t)
  {
    m_player->add_score(10);
  }
  old_t = t;
}

void play_state::draw_blip(jammy_game_object* h, int cell)
{
  vec2 d = h->get_pos() - m_player->get_pos();
  const float MAX_DIST = 200.f;
  float dist_sq = squared_length(d);
  if (dist_sq < MAX_DIST * MAX_DIST)
  {
    vec2 pos = RADAR_CENTRE + d * (10.f / MAX_DIST);
    m_blips.draw_cell(the_screen, cell, pos.x, pos.y);
  }
} 

void play_state::draw() 
{
  game_objects* gos = the_game.get_game_objects();
  for (game_object* go : *gos)
  {
    jammy_game_object* jgo = dynamic_cast<jammy_game_object*>(go);
    assert(jgo);
    jgo->draw(the_screen);
  }

  // Draw radar
  m_radar.blit(the_screen, RADAR_X, RADAR_Y);

  for (human* h : m_humans)
  {
    if (std::find(m_rescued_humans.begin(), m_rescued_humans.end(), h) == m_rescued_humans.end())
    {
      // Not rescued, so draw on radar
      draw_blip(h, 0);
    }
  }
  draw_blip(m_hq, 4);

  // Draw lives
  int lives = m_player->get_num_lives();
  for (int i = 0; i < lives; i++)
  {
    const int HEART_W = 10;
    m_life_full.blit(the_screen, screen::WIDTH - (i + 1) * HEART_W, 2);
  }

  // Draw score
  the_font.draw(the_screen, 1, 120, std::to_string(m_player->get_score()));
}

