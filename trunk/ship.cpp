#include <iostream>
#include "ship.h"
#include "math.h"
#include <GL/glut.h>
using namespace std;

Ship::Ship()
{
  hereIAm[0] =  hereIAm[1] = hereIAm[2] = 0.0;
  
  moving = jumping = false;

  cube = 0;
  angleZ = 0;
}

void Ship::drawShip()
{
  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();

  if(moving)
    move();

    glTranslatef(hereIAm[0],hereIAm[1],hereIAm[2]);

    /* Draw Cuboid "body" */
    glPushMatrix();
    glColor3f(1,0,0);
    glScalef(0.7,0.3,1.0); //Size of cuboid
    //glTranslatef(0.0,0.3,0.0);
    glutSolidCube(1);
    glPopMatrix();

    /* Draw Sphere "windshield" */
    glPushMatrix();
    glColor3f(0,1,0);
    glTranslatef(0.0,0.0,-0.3);
    glutSolidSphere(0.3, // Radius
	 	    10,  // slices 
		    10  // stacks
		    );

    glPopMatrix();

  glPopMatrix();
}

void Ship::moveHere(GLint cubeNR)
{
  cube = cube+cubeNR;
  // Convert to array values 0 - 4
  moving = true;
}


void Ship::move()
{
  /* Some calculations */

  // Distance we have left
  float distanceLeft = hereIAm[0] - cube;

  // Are we moving left or right?
  bool movingLeft = true;
  if (distanceLeft < 0)
    { movingLeft = false; }

  // Remove negative distance values
  distanceLeft = fabs(distanceLeft);

  /* Rotate when moving */
  if ( (distanceLeft > 0.5) && (angleZ < 50) )
    {
      angleZ = angleZ + 1;
      glRotatef(angleZ,
		hereIAm[0],
		0.0,
		1.0);
    }
  else if ( (distanceLeft < 0.5) && (angleZ > 0) )
    {
      angleZ = angleZ - 1;
      glRotatef(angleZ,
		hereIAm[0],
		0.0,
		1.0);
    }

  /* Move by the x axis a bit */
  if(movingLeft)
    { hereIAm[0] = hereIAm[0]-0.01; }
  else
    { hereIAm[0] = hereIAm[0]+0.01; }

  if (distanceLeft < 0.01)
    distanceLeft = 0.000;

  /* Have we arrived? */
  if (distanceLeft == 0)
    {
      hereIAm[0] = cube;
      angleZ = 0;
      moving = false;
    }
}
