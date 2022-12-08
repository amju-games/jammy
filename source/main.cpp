// "jammy" Game Prototype
// (c) Jason Colman 2017

#include <cassert>
#include <iostream>
#include <GLUT/glut.h>
#include "colour.h"
#include "font.h"
#include "game.h"
#include "global_palette.h"
#include "image.h"
#include "player.h"
#include "play_state.h"
#include "rock.h"
#include "screen.h"
#include "sprite.h"
#include "timer.h"

// Size of window in actual device pixels
const int WINDOW_W = 500;
const int WINDOW_H = 500;

const char BLACK = 1;

bool yes_full_screen = false;

screen the_screen;
game the_game;
timer the_timer;

player* the_player = nullptr;

//image im; // TEST
sprite spr;
font my_font;

void draw()
{
  glClear(GL_COLOR_BUFFER_BIT);

  the_screen.clear(BLACK);
  //im.blit(the_screen, 2, 60, 0);
  //spr.draw(the_screen, 2, 2);
  //my_font.draw(the_screen, 5, 5, "HELLO\n1234567890!@^&*()_+-=<>,.?/\"':;");

  the_game.draw();

  the_screen.draw_on_gl_thread(the_global_palette);

  glutSwapBuffers();
  glutPostRedisplay();
}

void update()
{
  the_timer.update();
  float dt = the_timer.get_dt();

  //spr.update(0);

  the_game.update(dt);
}

// * draw_and_update *
// Called every frame.
void draw_and_update()
{
  draw();
  update();
}

static int move = 0;

static void set(int& i, int b)
{
  i |= b;
}

static void clear(int& i, int b)
{
  i &= ~b; 
}

void key_down(unsigned char c, int, int)
{
  // Escape -> quit immediately
  if (c == 27)
  {
    if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE))
    {
      glutLeaveGameMode();
    }
    exit(0);
  }

  std::cout << "Got key down: " << c << "\n"; 
}

void key_up(unsigned char c, int, int)
{
  std::cout << "Got key up: " << c << "\n"; 
}

void special_key_down(int c, int, int)
{
//  std::cout << "Got special key down: " << c << "\n"; 

  switch (c)
  {
  case GLUT_KEY_UP:
    set(move, MOVE_UP);
    break;
  case GLUT_KEY_DOWN:
    set(move, MOVE_DOWN);
    break;
  case GLUT_KEY_LEFT:
    set(move, MOVE_LEFT);
    break;
  case GLUT_KEY_RIGHT:
    set(move, MOVE_RIGHT);
    break;
  }

  the_player->move(move);
}

void special_key_up(int c, int, int)
{
//  std::cout << "Got special key up: " << c << "\n"; 

  switch (c)
  {
  case GLUT_KEY_UP:
    clear(move, MOVE_UP);
    break;
  case GLUT_KEY_DOWN:
    clear(move, MOVE_DOWN);
    break;
  case GLUT_KEY_LEFT:
    clear(move, MOVE_LEFT);
    break;
  case GLUT_KEY_RIGHT:
    clear(move, MOVE_RIGHT);
    break;
  }

  the_player->move(move);
}

void joystick(unsigned int, int x, int y, int z)
{
  std::cout << "Got js callback! x: " << x << " y:" << y << "\n";
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

  // Joystick - Windows only?
  int joy_poll_interval = 60; // ?
  glutJoystickFunc(joystick, joy_poll_interval);

  gluOrtho2D(0, screen::WIDTH, 0, screen::HEIGHT);

  the_screen.set_size(screen::WIDTH, screen::HEIGHT);
  the_screen.clear(0);

  // Add black colour for space bg!
  // This will be index 1, because index 0 is for transparent colour.
  the_global_palette.add_colour(colour(0, 0, 0));

  //spr.load("../assets/test2.png", the_palette);

  //my_font.load("../assets/font1.png", the_palette);
  //my_font.set_num_cells(16, 4);

  the_player = new player;
  the_game.add_game_object(the_player);

  the_game.add_game_object(new rock);
  
  play_state* ps = new play_state;
  ps->set_game(&the_game);
  ps->set_screen(&the_screen);
  the_game.set_game_state(ps);

  // Must update once before draw
  update();

  glutMainLoop();
}


