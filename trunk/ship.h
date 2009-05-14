#ifndef SHIP_H
#define SHIP_H

#include "camera.h"
#include <GL/glut.h>

class Ship {
 public:
  Ship();
  void drawShip(Camera* cam);
  void moveHere(GLint cubeNr); // cube nr 1 - 5
  void jumpShip();

 private:
  void turn();
  void turnRotateZ(GLboolean movingLeft, GLfloat distanceLeft);
  void hover();
  void jump();
  void drawBody();
  void drawWindshield();

  GLdouble hereIAm[3];
  GLdouble cubePosition[5];
  GLint cube;
  GLboolean moving;
  GLboolean jumping;
  GLboolean turning;
  GLint hoverY;
  GLint angleZ;
  GLint angleX;
  GLfloat jumpDestination;
  GLfloat jumpHeight;
};

#endif
