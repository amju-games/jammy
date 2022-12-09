#include "directory.h"
#include "globals.h"
#include "splash_state.h"

splash_state::splash_state()
{
  m_image.load(get_data_dir() + "Splash_Screen.png", the_global_palette);
}

static float flash = 0;
static const float FLASH_PERIOD = 1.f;

void splash_state::update(float dt) 
{
  flash += dt;
  if (flash > FLASH_PERIOD)
  {
    flash = 0;
  }
}

void splash_state::draw()
{
  m_image.blit(the_screen, 0, 0);
  
  if (flash < FLASH_PERIOD * .5f)
  {
    the_font.draw(the_screen, 28, 45, "HIT SPACE TO START!");
  }
  the_font.draw(the_screen, 1, 120, "ARROW KEYS TO MOVE, ESC TO QUIT!");
}

void splash_state::on_input(int input) 
{
  the_game.set_game_state(the_play_state);
}

