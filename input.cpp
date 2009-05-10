#include "input.h"

#include <iostream>
#include "stdlib.h"
#include <GL/glut.h>
using namespace std;

void Input::normKey(unsigned char key, int mouseX, int mouseY)
{
  // Exit program if 'q' or ESCAPE is pressed.
  if(key == 'q' || key == 'Q' || key == '\27')
    exit(0);
}

void Input::specKey(int key, int mouseX, int mouseY)
{
  // if(key == GLUT_KEY_F1) /* do something */
  // if(key == GLUT_KEY_UP) /* ditto */
}
