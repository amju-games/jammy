#include "anim_controller.h"

void anim_controller_flying::update(float dt) 
{
  // Min speed for idle/not idle, so related to max speed
  const float DEAD_ZONE = 1.f;

  anim_state new_anim_state = m_anim_state;

  // TODO These ifs could be predicates in a container - we test each pred,
  //  and set the new state if the pred is true. This would be good as this
  //  type would be reusable and not care about the "business rules" of the
  //  game/player activity.

  // TODO also, maybe we should pass in the player object, not just the 
  //  vel, so we can test for other conditions, not just vel.

  // These ifs are all probably mutually exclusive but we don't really care,
  //  testing them all in this order gives us a priority order of anims
  
  if (m_vel.x > -DEAD_ZONE && m_vel.x < 0)
  {
    // small negative x vel, use idle left
    // Safer than checking for vel == 0, maybe it never gets there?!
    new_anim_state = anim_state::idle_left;
  }

  if (m_vel.x < DEAD_ZONE && m_vel.x > 0)
  {
    new_anim_state = anim_state::idle_right;
  }

  if (m_vel.x < -DEAD_ZONE) // && current_state is any
  {
    new_anim_state = anim_state::face_left;
  }

  if (m_vel.x > DEAD_ZONE) // && current_state is any
  {
    new_anim_state = anim_state::face_right;
  }

  // If we change anim_state, we can set the start cell in the new range to match
  //  the cell we were at in the previous range.

  // Changing state?
  if (m_anim_state == new_anim_state)
  {
    update_cell(dt);

  }
  else
  {
    // Is there a transition from current to new state?

    m_anim_state = new_anim_state;
    // TODO
    m_cell = get_anim_info().m_cell_min;
  }
}

void anim_controller_flying::update_cell(float dt)
{
  m_cell_time += dt;
  const auto& ai = get_anim_info();
  if (m_cell_time > ai.m_cell_max_time)
  {
    m_cell_time -= ai.m_cell_max_time;
    // Go to next cell.
    m_cell++;
    // Have reached end?
    if (m_cell > ai.m_cell_max)
    {
      // In looping state?
      if (ai.m_is_looping)
      {
        m_cell = ai.m_cell_min;
      }
      else
      {
        // Reached end of non-looping transitional state?
        m_cell = ai.m_cell_min; // TODO
      }
    }
  }
}

