#include <cassert>
#include <iostream>
#include <GLUT/glut.h>
#include "colour.h"
#include "directory.h"
#include "font.h"
#include "fps_counter.h"
#include "globals.h"
#include "image_32.h"
#include "input.h"
#include "jammy_blend.h"
#include "jammy_game_state.h"
#include "load_level_state.h"
#include "play_state.h"
#include "render_image_opengl.h"
#include "sound_player_bass24.h"
#include "splash_state.h"
#include "resources.h"
#include "universe.h"

bool yes_full_screen = false;

fps_counter the_fps_counter;

void draw()
{
  glClear(GL_COLOR_BUFFER_BIT);

  the_game.draw();

  the_font.draw<jb_font_mask>(the_screen, 20, 2, std::to_string(the_fps_counter.get_fps()) + "FPS");


  // Copy buffer to GL screen surface
  render_image_32_opengl(the_screen);

  glutSwapBuffers();
  glutPostRedisplay();
}

void update()
{
  the_timer.update();
  float dt = the_timer.get_dt();

  the_fps_counter.update(dt);
 
  the_game.update(dt);
}

// * draw_and_update *
// Called every frame.
void draw_and_update()
{
  draw();

  // Poll joystick
  glutForceJoystickFunc();

  update();
}

static int move = 0;

void key_down(unsigned char c, int, int)
{
#ifdef KEY_DEBUG
  std::cout << "Got key down: '" << c << "'\n"; 
#endif

  // Space bar => button
  if (c == ' ')
  {
    game_state* gs = the_game.get_game_state();
    jammy_game_state* jgs = dynamic_cast<jammy_game_state*>(gs);
    assert(jgs);
    jgs->on_input(BUTTON_A);
  }
  else if (c == 'R' || c == 'r')
  {
std::cout << "HOT RELOAD!\n";
    resources().reload();
  }
}

void key_up(unsigned char c, int, int)
{
#ifdef KEY_DEBUG
  std::cout << "Got key up: '" << c << "'\n"; 
#endif

  // Escape -> quit immediately
  // OH NO IT CRASHES

  if (c == 27)
  {
    the_sound_player.reset();

    if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE))
    {
      glutLeaveGameMode();
    }
    exit(0);
  }

}

void special_key_down(int c, int, int)
{
#ifdef KEY_DEBUG
  std::cout << "Got special key down: " << c << "\n"; 
#endif

  const button_value v = button_value::down;
  switch (c)
  {
  case GLUT_KEY_UP:
    the_game.on_dir_button_action({ dir_button_name::up, v });
    break;
  case GLUT_KEY_DOWN:
    the_game.on_dir_button_action({ dir_button_name::down, v });
    break;
  case GLUT_KEY_LEFT:
    the_game.on_dir_button_action({ dir_button_name::left, v });
    break;
  case GLUT_KEY_RIGHT:
    the_game.on_dir_button_action({ dir_button_name::right, v });
    break;
  }
}

void special_key_up(int c, int, int)
{
#ifdef KEY_DEBUG
  std::cout << "Got special key up: " << c << "\n"; 
#endif

  const button_value v = button_value::up;
  switch (c)
  { 
  case GLUT_KEY_UP:
    the_game.on_dir_button_action({ dir_button_name::up, v });
    break;
  case GLUT_KEY_DOWN:
    the_game.on_dir_button_action({ dir_button_name::down, v });
    break;
  case GLUT_KEY_LEFT:
    the_game.on_dir_button_action({ dir_button_name::left, v });
    break;
  case GLUT_KEY_RIGHT:
    the_game.on_dir_button_action({ dir_button_name::right, v });
    break;
  }

}

void joystick(unsigned int buttons, int x, int y, int z)
{
  the_game.on_joystick_action(joystick_action(
    static_cast<float>(x) / 1024.f,
    static_cast<float>(y) / 1024.f));

  // TODO Buttons
  return;

#ifdef JOYSTICK_DEBUG
  std::cout << "Got js callback! buttons: " << buttons 
    << "\tx: " << x 
    << "\ty: " << y 
    << "\tz: " << z 
    << "\n";
#endif

  // Initialise to first values, so we don't do anything on first call.
  static unsigned int prev_buttons = buttons;
  static int prev_x = x;
  static int prev_y = y;

  if (buttons == prev_buttons && x == prev_x && y == prev_y)
  {
    return;
  }

  game_state* gs = the_game.get_game_state();
  jammy_game_state* jgs = dynamic_cast<jammy_game_state*>(gs);
  assert(jgs);

  if (buttons != prev_buttons)
  {
    // Lowest 4 bits of move are movement directions. Shift button mask up.
    move &= 0x0f; // Save directions
    move |= ((buttons & 0x0fff) << 4); // copy button mask into move, shifted up 4 bits
    prev_buttons = buttons;
  }
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(WINDOW_W, WINDOW_H);

  if (yes_full_screen)
  {
    glutGameModeString("640x480:32@60");
    glutEnterGameMode();
  }
  else 
  {
    glutCreateWindow("Hello");
  }

  // Set up glut callbacks
  glutDisplayFunc(draw_and_update);
  glutKeyboardFunc(key_down);
  glutKeyboardUpFunc(key_up);
  glutSpecialFunc(special_key_down);
  glutSpecialUpFunc(special_key_up);

  // Joystick 
  const int POLL_WITH_GLUT_FORCE_JOYSTICK_FUNC = -1;
  glutJoystickFunc(joystick, POLL_WITH_GLUT_FORCE_JOYSTICK_FUNC);

  // Pretend screen size
  //gluOrtho2D(0, PRETEND_SCREEN_W, 0, PRETEND_SCREEN_H);
 
  the_screen = std::make_shared<image_32>();
  the_screen->set_size(PRETEND_SCREEN_W, PRETEND_SCREEN_H);

  // Add black colour for space bg!
  // This will be index 1, because index 0 is for transparent colour.
  //image_8::get_palette().add_colour(colour(0, 0, 0));

  // Add colour for rope - this is index 2.
  //image_8::get_palette().add_colour(colour(255, 255, 0));

  // Init font
  the_font.set_image(resources().get<image>(get_data_dir() + "font1 - magenta.png"));
  the_font.set_num_cells(16, 4);

  // Init game states
  the_play_state.reset(new play_state);
  the_load_level_state.reset(new load_level_state);
  the_splash_state.reset(new splash_state);
  the_game_over_state.reset(new game_over_state);
  the_enter_hi_score_state.reset(new enter_hi_score_state);

  // Initial state
  the_game.set_game_state(the_splash_state);

  // Must update once before draw
  update();

  the_sound_player.reset(new sound_player_bass24);

  // Play background music
  //const bool LOOP = true;
  //the_sound_player->play_wav(get_data_dir() + "Visager_-_11_-_Eerie_Mausoleum.wav", LOOP);

  //the_human_list.load();

  glutMainLoop();
}


