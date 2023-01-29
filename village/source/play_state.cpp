#include <iostream>
#include <bitset>

#include <cassert>
#include "blit.h"
#include "directory.h"
#include "game.h"
#include "globals.h"
#include "hq.h"
#include "human.h"
#include "human_list.h"
#include "input.h"
#include "jammy_blend.h"
#include "jammy_game_object.h"
#include "parallax_bg.h"
#include "player.h"
#include "player_bullet.h"
#include "play_state.h"
#include "resources.h"
#include "rock.h"
#include "string_utils.h"
#include "universe.h"

const int NUM_ROCKS = 1; 

//const float MAX_BIO_TIME = 3.f;
//const int NUM_HUMANS = 10; 
//const int PICK_UP_HUMAN_SCORE = 250;
//const int DELIVER_HUMAN_SCORE = 1000;

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

void play_state::on_dir_button_action(const dir_button_action& dba)
{
  m_player->get_vel_controller().on_dir_button_action(dba);
}

void play_state::on_input(int input)
{
  assert(m_player);

  if (input & BUTTON_A)
  {
    add_player_bullet();
  }
}

void play_state::add_player_bullet()
{
  // Use a circular buffer for player bullets.
  // If we haven't filled up the buffer, create a new player_bullet, add it to the game and
  //  the buffer. If we have filled the buffer, reset the player_bullet we are currently pointing
  //  to, and inc to the next slot.

  auto pb = m_player_bullets->get_next_element();
  pb->fire(m_player);

  // TODO Appropriate sound FX
  the_sound_player->play_wav(get_data_dir() + "sounds/sfx_sounds_impact3.wav");
}

void play_state::dec_num_rocks()
{
  assert(m_num_rocks_in_level > 0);
  m_num_rocks_in_level--;
  
std::cout << "This many rocks left in level: " << m_num_rocks_in_level << "\n";

  if (m_num_rocks_in_level == 0)
  {
    // TODO New level
  }
}

std::shared_ptr<rock> play_state::add_rock_and_descendants(int level, int child_index)
{
  auto r = std::make_shared<rock>(level, child_index);
  the_game.add_game_object(r);
  m_rocks.push_back(r);
  m_num_rocks_in_level++;

  if (level < 2)
  {
    for (int i = 0; i < 2; i++) // TODO a bit of variety in num children
    {
      // Pass in i so each child can have a different sprite sheet
      auto child = add_rock_and_descendants(level + 1, i);
      r->add_child(child);
    }
  }

  return r;
}

void play_state::on_active() 
{
  the_sound_player->play_wav(get_data_dir() + "sounds/sfx_sounds_powerup2.wav");

  const int MAX_PLAYER_BULLETS = 10;
  m_player_bullets = std::make_unique<circular_buffer<player_bullet>>(the_game, MAX_PLAYER_BULLETS);

  m_player = nullptr; 
  m_humans.clear();
  m_rescued_humans.clear();
  m_rocks.clear();
  the_game.clear_game_objects();

  // Add background
  the_game.add_game_object(std::make_shared<parallax_bg>());
  
  // Add HQ
//  m_hq = new hq;
//  the_game.add_game_object(std::shared_ptr<hq>(m_hq));

//  // Add humans
//  for (int i = 0; i < NUM_HUMANS; i++)
//  {
//    human* h = new human;
//    the_game.add_game_object(std::shared_ptr<human>(h));
//    m_humans.push_back(h);
//  }

  // Add player
  m_player = std::make_shared<player>();
  the_game.add_game_object(m_player);

  // Add asteroids
  for (int i = 0; i < NUM_ROCKS; i++)
  {
    // Rocks break up into child rocks. Let's do this recursively.
    add_rock_and_descendants(0, 0); // level 0: largest rock; index 0
  }

  populate_collision_funcs(m_collision_mgr);
  m_collision_mgr.set_game_objects(the_game.get_game_objects());
}

void play_state::on_deactive() 
{
}

void play_state::col_det()
{
  m_collision_mgr.check_for_collisions();

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
*/
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
    m_blips.draw_cell<jb_mask>(the_screen, cell, pos.x, pos.y);
  }
} 

void play_state::draw_radar()
{
  // Draw radar
  blit<jb_mask>(m_radar, the_screen, RADAR_X, RADAR_Y);

  for (human* h : m_humans)
  {
    if (std::find(m_rescued_humans.begin(), m_rescued_humans.end(), h) == m_rescued_humans.end())
    {
      // Not rescued, so draw on radar
      draw_blip(h, 0);
    }
  }
  draw_blip(m_hq, 4);
}
 
void play_state::draw_lives()
{
  int lives = m_player->get_num_lives();
  for (int i = 0; i < lives; i++)
  {
    const int HEART_W = 10;
    blit<jb_mask>(m_life_full, the_screen, PRETEND_SCREEN_W- (i + 1) * HEART_W, 2);
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

  draw_lives();
  
  // Draw score
  the_font.draw<jb_font_mask>(the_screen, 1, 1, std::to_string(m_player->get_score()));

  // Draw human bio
//  if (human_timer > 0)
//  {
//    the_human_list.draw_human_bio(human_to_display);
//  } 

//  the_font.draw<jb_font_mask>(the_screen, 20, 8,  concat("PLR: ", m_player->get_pos()));
//  the_font.draw<jb_font_mask>(the_screen, 20, 16,  concat("HQ:  ", m_hq->get_pos()));
//  the_font.draw<jb_font_mask>(the_screen, 20, 16, concat("VEL: ", m_player->get_vel()));
//  the_font.draw<jb_font_mask>(the_screen, 20, 24, concat("ACC: ", m_player->get_acc()));

}

