#include <iostream>
#include <cassert>
#include "directory.h"
#include "game.h"
#include "globals.h"
#include "hq.h"
#include "human.h"
#include "human_list.h"
#include "input.h"
#include "jammy_game_object.h"
#include "parallax_bg.h"
#include "player.h"
#include "player_bullet.h"
#include "play_state.h"
#include "resources.h"
#include "rock.h"
#include "screen.h"
#include "string_utils.h"

const float MAX_BIO_TIME = 3.f;

const int NUM_ROCKS = 20; 
const int NUM_HUMANS = 10; 

const int PICK_UP_HUMAN_SCORE = 250;
const int DELIVER_HUMAN_SCORE = 1000;

const int RADAR_X = 0;
const int RADAR_Y = 0;
const vec2 RADAR_CENTRE(RADAR_X + 8, RADAR_Y + 8);

float human_timer = 0;
int human_to_display = 0;

play_state::play_state()
{
  m_life_empty = resources().get<image>(get_data_dir() + "life_empty.png");
  m_life_full = resources().get<image>(get_data_dir() + "life_full.png");

  m_radar = resources().get<image>(get_data_dir() + "radar.png");
  m_blips.set_image(resources().get<image>(get_data_dir() + "blips.png"));
  m_blips.set_num_cells(3, 2);
}

void play_state::on_input(int input)
{
  assert(m_player);

  // If this input is a combination of move directions, values will be 0..0xf.
  // BUTTON_A is 0x10
/*
  if (input & 0x0f && !m_player->is_immune())
  {
    m_player->move(input & 0x0f);  
  }
*/
  if (input & BUTTON_A)
  {
    add_player_bullet();
  }
  else
  {
    m_player->move(input & 0x0f);  
  }
}

void play_state::add_player_bullet()
{
  // For bullet dir, we use last non-zero direction of player
  player_bullet* pb = new player_bullet(m_player->get_pos(), m_player->get_last_move_dir());
  the_game.add_game_object(std::shared_ptr<player_bullet>(pb));
  m_player_bullets.push_back(pb);

  the_sound_player->play_wav(get_data_dir() + "sounds/sfx_sounds_impact3.wav");
}

void play_state::on_active() 
{
  the_sound_player->play_wav(get_data_dir() + "sounds/sfx_sounds_powerup2.wav");

  m_player = nullptr; 
  m_humans.clear();
  m_rescued_humans.clear();
  m_rocks.clear();
  the_game.clear_game_objects();

  // Add background
  the_game.add_game_object(std::make_shared<parallax_bg>());
  
  // Add HQ
  m_hq = new hq;
  the_game.add_game_object(std::shared_ptr<hq>(m_hq));

  // Add humans
  for (int i = 0; i < NUM_HUMANS; i++)
  {
    human* h = new human;
    the_game.add_game_object(std::shared_ptr<human>(h));
    m_humans.push_back(h);
  }

  // Add player
  m_player = new player;
  the_game.add_game_object(std::shared_ptr<player>(m_player));

  // Add asteroids
//  for (int i = 0; i < NUM_ROCKS; i++)
//  {
//    rock* r = new rock;
//    the_game.add_game_object(r);
//    m_rocks.push_back(r);
//  }

  populate_collision_funcs(m_collision_mgr);
  m_collision_mgr.set_game_objects(the_game.get_game_objects());
}

void play_state::on_deactive() 
{
}

void play_state::col_det()
{
//  m_collision_mgr.check_for_collisions();

/*
  // Test player and humans against rocks
  for (rock* r : m_rocks)
  {
    // Check player/rock sprites
    if (sprite_collision(r, m_player))
    {
      if (m_player->is_immune())
      {
        m_player->keep_immune();
      }
      else
      {
        m_player->lose_life();
        the_sound_player->play_wav(get_data_dir() + "sounds/Explosion2.wav");
      }
    }

    // Check for rocks v humans
    for (auto it = m_rescued_humans.begin(); it != m_rescued_humans.end(); ++it)
    {
      human* h = *it;
      if (sprite_collision(r, h))
      {
        // This human is separated from its parent in the chain
        h->set_rescued(false);
        h->set_parent(nullptr);

        the_sound_player->play_wav(get_data_dir() + "sounds/sfx_sounds_impact3.wav");

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
*/

  // Test for humans
  int i = 0;
  for (human* h : m_humans)
  {
    if (sprite_collision(h, m_player))
    {
      // Already rescued?
      if (std::find(m_rescued_humans.begin(), m_rescued_humans.end(), h) == m_rescued_humans.end())
      {
        // Not already rescued, so add to chain
        m_player->add_score(PICK_UP_HUMAN_SCORE);
        the_sound_player->play_wav(get_data_dir() + "sounds/sfx_sounds_fanfare2.wav");

        human_timer = MAX_BIO_TIME;
        human_to_display = i;

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
    i++;
  }

  // Check for rescued humans reaching the space ship (HQ)
  // Check for collision with HQ
  for (auto it = m_rescued_humans.begin(); it != m_rescued_humans.end(); ++it)
  {
    human* h = *it;
  
    if (sprite_collision(h, m_hq))
    {
      the_sound_player->play_wav(get_data_dir() + "sounds/Sweep2.wav");

      // This human and all descendants are delivered to the ship!
      for (auto jt = it; jt != m_rescued_humans.end(); ++jt)
      {
        m_player->add_score(DELIVER_HUMAN_SCORE);
        m_player->add_human_saved();

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

  if (!m_player->is_immune() && m_player->get_num_lives() < 1)
  {
    the_game.set_game_state(the_game_over_state);
    the_sound_player->play_wav(get_data_dir() + "sounds/Shut_Down1.wav");
  }

  // Get a score for being alive
  if (!m_player->is_immune())
  {
    static float t = 0; 
    static float old_t = t;
    t += dt;
    if ((int)t != (int)old_t)
    {
      m_player->add_score(10);
    }
    old_t = t;
  }

  if (human_timer > 0)
  {
    human_timer -= dt;
  } 
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
  for (p_game_object& go : *gos)
  {
    jammy_game_object* jgo = dynamic_cast<jammy_game_object*>(go.get());
    assert(jgo);
    jgo->draw(the_screen);
  }

  // Draw radar
  m_radar->blit(the_screen, RADAR_X, RADAR_Y);

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
    m_life_full->blit(the_screen, screen::WIDTH - (i + 1) * HEART_W, 2);
  }

  // Draw score
  the_font.draw(the_screen, 1, 120, std::to_string(m_player->get_score()));

  // Draw human bio
//  if (human_timer > 0)
//  {
//    the_human_list.draw_human_bio(human_to_display);
//  } 

  the_font.draw(the_screen, 20, 8,  concat("PLR: ", m_player->get_pos()));
  the_font.draw(the_screen, 20, 16,  concat("HQ:  ", m_hq->get_pos()));
//  the_font.draw(the_screen, 20, 16, concat("VEL: ", m_player->get_vel()));
//  the_font.draw(the_screen, 20, 24, concat("ACC: ", m_player->get_acc()));

}

