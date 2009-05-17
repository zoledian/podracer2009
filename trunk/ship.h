#ifndef SHIP_H
#define SHIP_H

#include "camera.h"
#include <GL/glut.h>
#include <math.h>

class Ship {
 public:
  Ship(Camera* cam);
  void drawShip(GLdouble yDistance, GLdouble angle);
  void moveHere(GLint cubeNr);
  void jumpShip();
  GLdouble* getPosition();

 private:
  void turn();
  void turnRotateZ(GLboolean movingLeft, GLdouble distanceLeft);
  void wiggle();
  void hover();
  void jump();
  void gravity(GLdouble yDistance, GLdouble angle);
  void printHighscore();

  void drawBody();
  void drawWindshield();

  void printDebug();

  GLuint loadTexture(char* name);

  /* Physics variables */
  GLdouble jumpLength;
  GLdouble hoverHeight;
  GLdouble velocity;
  GLdouble turnSpeed;

  /* Helper variables */
  // Ship specific
  GLdouble _location[3];
  GLint _cubeNr; // Cube we are standing on (x-wise) 0 = middle -2 = leftmost
  GLdouble _angle;

  // States
  GLboolean _moving;
  GLboolean _jumping;
  GLboolean _turning;
  GLboolean _wiggle;
  GLboolean _falling;

  // Misc
  GLboolean _wiggleToRight;

  GLdouble _hoverY;
  GLint _hoverCounter;

  GLint _turnAngle;
  GLint _wiggleAngle;
  GLdouble _wiggleX;
  GLint _wiggleCounter;

  GLint angleX;

  GLdouble jumpDestination;
  GLdouble jumpHeight;

  GLdouble currentSpeed;

  Camera* _camera;

  GLuint textureId;

  GLdouble hereWeDie; // FIXME
};

#endif
