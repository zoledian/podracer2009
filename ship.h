#ifndef SHIP_H
#define SHIP_H

#include "camera.h"
#include <GL/glut.h>
#include <math.h>
#include "particlesystem.h"

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
  void printLevelComplete();

  GLuint loadTexture(char* name);

  /* Ship objects */
  GLUquadricObj* shipBody_;
  GLUquadricObj* shipWindshield_;
  GLUquadricObj* shipEngine_;
  GLUquadricObj* shipEngineBack_;

  /* Physics variables */
  GLdouble jumpLength_;
  GLdouble hoverHeight_;
  GLdouble velocity_;
  GLdouble turnSpeed_;

  /* Helper variables */
  // Ship specific
  GLdouble location_[3];
  GLint cubeNr_; // Cube we are standing on (x-wise) 0 = middle -2 = leftmost
  GLdouble shipAngleX_;

  // States
  GLboolean moving_;
  GLboolean jumping_;
  GLboolean turning_;
  GLboolean wiggling_;
  GLboolean falling_;
  GLboolean warping_;
  GLboolean intro_;

  // ParticleSystem for engines
  ParticleSystem* flame1_;
  ParticleSystem* flame2_;

  // Misc
  GLboolean wiggleToRight_;
  GLint wiggleAngle_;
  GLdouble wiggleX_;
  GLint wiggleCounter_;

  GLdouble hoverY_;
  GLint hoverCounter_;

  GLint turnAngle_;

  GLint jumpAngleX_;
  GLdouble jumpDestination_;

  GLdouble currentSpeed_;

  Camera* camera_;

  GLuint textureId;
  GLuint textureId2;

  GLdouble hereWeDie_;

  GLdouble jumpOrigin_;
  GLdouble jumpLengthOriginal_;

  GLdouble shipFumes_;
  GLdouble warpCounter_;
  GLdouble highscore_;

};

#endif
