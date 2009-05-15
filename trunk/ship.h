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
  GLdouble* getPosition();

 private:
  void turn();
  void turnRotateZ(GLboolean movingLeft, GLfloat distanceLeft);
  void turnEnd();
  void hover();
  void jump();
  void drawBody();
  void drawWindshield();

  /* Physics variables */
  GLfloat jumpLength;
  GLfloat hoverHeight;
  GLfloat velocity;
  GLfloat turnSpeed;

  /* Helper variables */
  GLdouble hereIAm[3];
  GLint cube; // Cube we are standing on (x-wise) 0 = middle -2 = leftmost

  GLboolean moving;
  GLboolean jumping;
  GLboolean turning;
  GLboolean turningEnd;
  GLboolean turningEndDirectionIsRight;

  GLfloat hoverY;
  GLint hoverCounter;
  GLint turnAngle;
  GLint turnEndAngle;
  GLdouble turnEndX;
  GLint turnEndCounter;
  GLint angleX;

  GLfloat jumpDestination;
  GLfloat jumpHeight;

  GLfloat currentSpeed;
};

#endif
