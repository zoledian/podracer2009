#include "camera.h"
#include <iostream>
#include <GL/glut.h>
#include <math.h>
using namespace std;

Camera::Camera()
{
  _speed = 0.05;

  _location[0] = 0.0;
  _location[1] = 5.0;
  _location[2] = 3.0;

  slowZ = false;

}

void Camera::LookAtThis(GLdouble x, GLdouble y, GLdouble z)
{

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90, 1, 0.01, 100);

  if (x < (0.95*_location[0]))
    _location[0] = _location[0] - _speed;
  if (x > (1.05*_location[0]))
    _location[0] = _location[0] + _speed;
  if (y < (0.90*(_location[1]-5.0)))
    _location[1] = _location[1] - 0.6*_speed;
  if (y > (1.10*(_location[1]-5.0)))
    _location[1] = _location[1] + 0.6*_speed;

  if (slowZ)
    {
      if (z < (0.95 * (_location[2] - 3.0) ))
	_location[2] = _location[2] - _speed*2;
      if (z > (1.05 * (_location[2] - 3.0) ))
	_location[2] = _location[2] + _speed*2;
      if ( (fabs( z - (_location[2] - 3.0)) < 20)
	   && (fabs( z - (_location[2] - 3.0)) > 5))
	_location[0] = _location[0] + _speed*1.5;
    }
  else
    _location[2] = z + 3.0;

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(_location[0], _location[1], _location[2],
	    x, y, z-3,
	    0, 1, 0);
}

GLdouble* Camera::getLocation()
{
  return _location;
}
