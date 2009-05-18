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
  void gravity(GLdouble yDistance);

  void jump();

  void turn();
  void turnRotateZ(GLboolean movingLeft, GLdouble distanceLeft);

  void wiggle();

  void hover();

  void drawBody();
  void drawWindshield();

  void printHighscore();

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
  GLdouble _shipAngleX;

  // States
  GLboolean _moving;
  GLboolean _jumping;
  GLboolean _turning;
  GLboolean _wiggle;
  GLboolean _falling;

  // Misc
  GLboolean _wiggleToRight;
  GLint _wiggleAngle;
  GLdouble _wiggleX;
  GLint _wiggleCounter;

  GLdouble _hoverY;
  GLint _hoverCounter;

  GLint _turnAngle;

  GLint _jumpAngleX;
  GLdouble _jumpDestinationZ;

  GLdouble _currentSpeed;

  Camera* _camera;

  GLuint textureId;

  GLdouble hereWeDie; // FIXME

  GLdouble _yDistance; // FIXME
};

#endif
