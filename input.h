#ifndef INPUT_H
#define INPUT_H

#include <GL/glut.h>

class Input {
 public:
  // Input();
  static void normKey(unsigned char key, int mouseX, int mouseY);
  static void specKey(int key, int mouseX, int mouseY);

};

#endif
