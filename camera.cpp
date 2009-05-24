#include "camera.h"
#include <iostream>
#include <GL/glut.h>
#include <math.h>
using namespace std;

Camera::Camera()
{
  speed_ = 0.05;

  location_[0] = 0.0;
  location_[1] = 0.0;
  location_[2] = 0.0;

  // slowZ indicates whether to travel slowly (such when you die) 
  // or instantanious (normal gameplay)
  slowZ = false;

  // still indicates whether the camera should stand still 
  still = false;

  // turning indicates that the camera is turning, which is used by 
  // loadlevel to determine which blocks to draw
  turning = false;

  aspectRatio_ = 1;

}

void Camera::reset()
{
  location_[0] = 5.0;
  location_[1] = 5.0;
  location_[2] = 10.0;
}

void Camera::updateAspect(double width, double height)
{
  aspectRatio_ = width / height;
}

void Camera::LookAtThis(GLdouble x, GLdouble y, GLdouble z)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90, aspectRatio_, 0.01, 100);

  if (still == false)
    {
      // Make the camera lag behind a bit when moving in x
      if (x < (0.95*location_[0]))
	location_[0] = location_[0] - speed_;
      if (x > (1.05*location_[0]))
	location_[0] = location_[0] + speed_;

      location_[1] = y + 5;

      // slowZ is active when you die and the camera pans back to start.
      if (slowZ)
	{
	  if (z < (0.95 * (location_[2] - 3.0) ))
	    location_[2] = location_[2] - speed_*2;
	  if (z > (1.05 * (location_[2] - 3.0) ))
	    location_[2] = location_[2] + speed_*2;
	  
	  // Go around the spaceship, not over
	  if ( (fabs( z - (location_[2] - 3.0)) < 20)
	       && (fabs( z - (location_[2] - 3.0)) > 5))
	    {
	      turning = true;
	      location_[0] = location_[0] + speed_*1.5;
	    }
	}
      else
	{
	  location_[2] = z + 3.0;
	}
    }

  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();
  gluLookAt(location_[0], location_[1], location_[2],
	    x, y, z-3,
	    0, 1, 0);

}

GLdouble* Camera::getLocation()
{
  return location_;
}
