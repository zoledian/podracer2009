#include "camera.h"
#include <GL/glut.h>
using namespace std;

Camera::Camera()
{
  LookAtThis(0.0,0.0,0.0);
}

void Camera::LookAtThis(GLdouble x, GLdouble y, GLdouble z)
{

  //glPushMatrix();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(75, 1, 0.01, 100);
  //glPopMatrix();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(x, 5, 3,
	    x, y, z-3,
	    0, 1, 0);
}
