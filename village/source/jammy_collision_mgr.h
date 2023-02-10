#pragma once

#include <memory>

class jammy_collision_mgr_impl;

class jammy_collision_mgr 
{
public:
  jammy_collision_mgr();
  void check_for_collisions();
  void set_game_objects(const game_objects*);
  void populate_collision_funcs();

private:
  jammy_collision_mgr_impl* m_pimpl;
  //std::unique_ptr<jammy_collision_mgr_impl> m_pimpl;
};

