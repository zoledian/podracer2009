#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glut.h>

class Camera {
 public:
  Camera();
  void LookAtThis(GLdouble x, GLdouble y, GLdouble z);
  GLboolean slowZ;
  GLboolean still;
  GLboolean turning;
  void reset();
  GLdouble* getLocation();
  void updateAspect(double width, double height);

 private:
  GLdouble location_[3];
  GLdouble speed_;
  double aspectRatio_;

};

#endif
