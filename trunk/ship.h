#ifndef SHIP_H
#define SHIP_H

#include <GL/glut.h>

class Ship {
 public:
  Ship();
  static void specKey(int key, int mouseX, int mouseY);
  void moveShip(GLdouble x, GLdouble y, GLdouble z);

 private:
  void drawShip(GLdouble x, GLdouble y, GLdouble z);  
};

#endif
