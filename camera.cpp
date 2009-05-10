#include "camera.h"
#include <GL/glut.h>
using namespace std;

Camera::Camera()
{
  LookAtThis(0.0,0.0,0.0);

}

void Camera::LookAtThis(GLdouble x, GLdouble y, GLdouble z)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt( x, y+10, z-10,
	     x, y, z,
	     0.0, 1.0, 0.0 );
}
