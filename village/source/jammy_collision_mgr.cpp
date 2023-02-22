#include "brute_force.h"
#include "collision_mgr.h"
#include "collision_player_rock.h"
#include "collision_player_bullet_moon.h"
#include "collision_player_bullet_rock.h"
#include "double_dispatcher.h"
#include "jammy_collision_mgr.h"
#include "narrow_phase_pixel_perfect.h"
#include "sweep_and_prune.h"

// Colliding types (fwd declared in collision headers)
#include "moon.h"
#include "player.h"
#include "player_bullet.h"
#include "rock.h"

//#define USE_SAP
#define USE_BRUTE_FORCE

using dd = double_dispatcher<jammy_game_object>;

#ifdef USE_BRUTE_FORCE
using jammy_collision_mgr_impl_base =
  collision_mgr<
    dd,
    brute_force<jammy_game_object, dd>,
    narrow_phase_pixel_perfect<jammy_game_object>
  >;
#endif

#ifdef USE_SAP
using jammy_collision_mgr_impl_base =
  collision_mgr<
    dd,
    sweep_and_prune<2, jammy_game_object, dd>,
    narrow_phase_pixel_perfect<jammy_game_object>
  >;
#endif

class jammy_collision_mgr_impl : public jammy_collision_mgr_impl_base
{
};

jammy_collision_mgr::jammy_collision_mgr()
{
  //m_pimpl = std::make_unique<jammy_collision_mgr_impl>();
  m_pimpl = new jammy_collision_mgr_impl;
}

void jammy_collision_mgr::check_for_collisions()
{
  m_pimpl->check_for_collisions();
}

void jammy_collision_mgr::set_game_objects(const game_objects* gos)
{
  m_pimpl->set_game_objects(gos);
}

void jammy_collision_mgr::populate_collision_funcs()
{
  m_pimpl->add_handler<player, rock>(collision_player_rock);
  m_pimpl->add_handler<player_bullet, moon>(collision_player_bullet_moon);
  m_pimpl->add_handler<player_bullet, rock>(collision_player_bullet_rock);
}


