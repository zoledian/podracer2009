#ifndef SHIP_H
#define SHIP_H

#include <GL/glut.h>

class Ship {
 public:
  Ship();
  void drawShip();  
  void moveHere(GLint cubeNr); // cube nr 1 - 5

 private:
  void move();

  GLdouble hereIAm[3];
  GLdouble cubePosition[5];
  GLint cube;
  GLboolean moving;
  GLboolean jumping;
  GLint angleZ;
};

#endif
