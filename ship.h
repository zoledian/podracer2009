#ifndef SHIP_H
#define SHIP_H

#include "camera.h"
#include <GL/glut.h>
#include <math.h>

class Ship {
 public:
  Ship(Camera* cam);
  void display(GLdouble yDistance, GLdouble angle, int blockType);
  void moveHere(GLint cubeNr);
  void jumpShip();
  GLdouble* getPosition();
  GLboolean readyForNextLevel;
  void reset();

 private:
  void drawShip();
  void gravity(GLdouble yDistance, GLdouble blockAngle);
  void jump();
  void turn();
  void turnRotateZ(GLboolean movingLeft, GLdouble distanceLeft);
  void wiggle();
  void hover();
  void warp();
  void intro();

  void drawBody();
  void drawEngine(GLint nr);
  void drawWindshield();

  void printHighscore();

  GLuint loadTexture(char* name);

  /* Ship objects */
  GLUquadricObj* _shipBody;
  GLUquadricObj* _shipWindshield;
  GLUquadricObj* _shipEngine;
  GLUquadricObj* _shipEngineBack;

  /* Physics variables */
  GLdouble jumpLength;
  GLdouble hoverHeight;
  GLdouble velocity;
  GLdouble turnSpeed;

  /* Helper variables */
  // Ship specific
  GLdouble _location[3];
  GLdouble _locationOfFront[3];
  GLint _cubeNr; // Cube we are standing on (x-wise) 0 = middle -2 = leftmost
  GLdouble _shipAngleX;

  // States
  GLboolean _moving;
  GLboolean _jumping;
  GLboolean _turning;
  GLboolean _wiggle;
  GLboolean _falling;
  GLboolean _warping;
  GLboolean _intro;

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
  GLuint textureId2;

  GLdouble hereWeDie; // FIXME

  GLdouble _jumpOrigin;
  GLdouble _jumpLengthOriginal;

  GLdouble _shipFumes;
  GLdouble _warpCounter;
  GLdouble _highscore;

};

#endif
