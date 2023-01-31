#include <cassert>
#include <iostream>
#include <GLUT/glut.h>
#include "auto_repeat_suppressor.h"
#include "globals.h"
#include "init_state.h"
#include "render_image_opengl.h"
#include "universe.h"

//#define KEY_DEBUG

std::unique_ptr<init_state> the_init_state;

bool yes_full_screen = false;

void draw()
{
  the_game.draw();

  // Copy buffer to GL screen surface
  render_image_32_opengl(the_screen);

  glutSwapBuffers();
  glutPostRedisplay();
}

void update()
{
  the_timer.update();
  float dt = the_timer.get_dt();

  the_game.update(dt);
}

void draw_and_update()
{
  draw();

  // Poll joystick
  glutForceJoystickFunc();

  update();
}

static auto_repeat_suppressor the_auto_repeat_suppressor;

void key_down(unsigned char c, int, int)
{
#ifdef KEY_DEBUG
  std::cout << "Got key down: '" << c << "'\n"; 
#endif

  keyboard_action ka(c, button_value::down);
  if (the_auto_repeat_suppressor.is_new_event(ka))
  {
    the_game.on_keyboard_action(ka);
  }
}

void key_up(unsigned char c, int, int)
{
#ifdef KEY_DEBUG
  std::cout << "Got key up: '" << c << "'\n"; 
#endif

  keyboard_action ka(c, button_value::up);
  if (the_auto_repeat_suppressor.is_new_event(ka))
  {
    the_game.on_keyboard_action(ka);
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

// Convert glut bitfield to button actions.
// TODO Currently discards events if more than one button changes this frame
game_controller_button_action glut_button_bitfield_to_button_action(unsigned int buttons)
{
  static unsigned int  prev_value = 0;
  unsigned int changed = buttons ^ prev_value;
  prev_value = buttons;

  int bit = 1;
  int button = 0;
  while (changed)
  {
    if (changed & bit) 
    {
      button_value bv = (buttons & bit) ? button_value::down : button_value::up;

#ifdef KEY_DEBUG
std::cout << "Button " << button << ((bv == button_value::down) ? "down" : "up") << "\n";
#endif

      return game_controller_button_action{button, bv};
    }
    bit <<= 1;
    button++;
  }

  return game_controller_button_action{};
}

void joystick(unsigned int buttons, int x, int y, int z)
{
  the_game.on_joystick_action(joystick_action(
    static_cast<float>(x) / 1024.f,
    static_cast<float>(y) / 1024.f));

  // TODO we could have multiple button actions, for every changed bit
  auto gcba = glut_button_bitfield_to_button_action(buttons);
  the_game.on_game_controller_button_action(gcba);
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

  // Initial state
  the_init_state = std::make_unique<init_state>();
  the_game.set_game_state(the_init_state);

  // Must update once before draw
  update();

  glutMainLoop();
}


