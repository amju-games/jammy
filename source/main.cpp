// "jammy" Game Prototype
// (c) Jason Colman 2017

#include <array>
#include <cassert>
#include <iostream>
#include <GLUT/glut.h>
#include "colour.h"
#include "font.h"
#include "image.h"
#include "palette.h"
#include "screen.h"
#include "sprite.h"

// Size of window in actual device pixels
const int WINDOW_W = 500;
const int WINDOW_H = 500;

bool yes_full_screen = false;

screen the_screen;
palette the_palette;

//image im; // TEST
sprite spr;
font my_font;

void draw()
{
  glClear(GL_COLOR_BUFFER_BIT);

  the_screen.clear(0); // black
  //im.blit(the_screen, 2, 60, 0);
  spr.draw(the_screen, 2, 2);
  my_font.draw(the_screen, 5, 5, "HELLO\n1234567890!@^&*()_+-=<>,.?/\"':;");

  the_screen.draw_on_gl_thread(the_palette);

  glutSwapBuffers();
  glutPostRedisplay();
}

void update()
{
  spr.update(0);
}

// * draw_and_update *
// Called every frame.
void draw_and_update()
{
  draw();
  update();
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
  std::cout << "Got special key down: " << c << "\n"; 
}

void special_key_up(int c, int, int)
{
  std::cout << "Got special key up: " << c << "\n"; 
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

  the_palette.add_colour(colour(0, 0, 0));

  spr.load("../assets/test2.png", the_palette);

  my_font.load("../assets/font1.png", the_palette);
  my_font.set_num_cells(16, 4);

  glutMainLoop();
}


