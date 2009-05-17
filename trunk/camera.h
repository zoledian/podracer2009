#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glut.h>

class Camera {
 public:
  Camera();
  void LookAtThis(GLdouble x, GLdouble y, GLdouble z);
  GLboolean slowZ;
  GLdouble* getLocation();

 private:
  GLdouble _location[3];
  GLdouble _speed;

};

#endif
