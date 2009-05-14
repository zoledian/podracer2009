#include <iostream>
#include "ship.h"
#include "math.h"
#include <GL/glut.h>
using namespace std;

Ship::Ship()
{
  hereIAm[0] =  hereIAm[1] = hereIAm[2] = 0.0;
  
  moving = turning = jumping = false;

  cube = 0; // 0 = middle -2 = leftmost 2 = rightmost
  angleZ = hoverY = 0;
}

void Ship::drawShip()
{
  glMatrixMode(GL_MODELVIEW);

  // Translate
  glTranslatef(hereIAm[0],hereIAm[1],hereIAm[2]);

  // Rotate
  if(turning)
    turn();
  else
    hover();

  // Draw
  drawBody();
  drawWindshield();

}

void Ship::moveHere(GLint cubeNR)
{
  // Keep ship from falling down on the sides
  if (fabs(cube+cubeNR) < 3)
    {
      cube = cube+cubeNR;
      turning = true;
    }
}


void Ship::turn()
{
  // Distance we have left
  GLfloat distanceLeft = hereIAm[0] - cube;

  // Are we moving left or right?
  GLboolean movingLeft = true;
  if (distanceLeft < 0)
    { movingLeft = false; }

  // Remove negative distance values
  distanceLeft = fabs(distanceLeft);

  // Rotate when moving
  turnRotateZ(movingLeft, distanceLeft);

  // Move by the x axis a bit
  if(movingLeft)
    { hereIAm[0] = hereIAm[0]-0.025; }
  else
    { hereIAm[0] = hereIAm[0]+0.025; }

  // Make floats behave!
  if (distanceLeft < 0.01)
    distanceLeft = 0.000;

  // Have we arrived?
  if (distanceLeft == 0)
    {
      hereIAm[0] = cube;
      angleZ = 0;
      turning = false;
    }
}

void Ship::turnRotateZ(GLboolean movingLeft, GLfloat distanceLeft)
{
  if ( !movingLeft )
    {
      if ( (distanceLeft < 0.5) && (angleZ > 0) )
	{
	  // We are moving right and are close to our destination
	  angleZ = angleZ - 1;
	}
      else if (angleZ < 30)
	{
	  // We are moving right and are far away from our destination
	  angleZ = angleZ + 1;
	}
    }
  else if (movingLeft)
    {
      if ( (distanceLeft < 0.5) && (angleZ < 0) )
	{
	  // We are moving left and are close to our destination
	  angleZ = angleZ + 1;
	}
      else if (angleZ > -30)
	{
	  // We are moving left and are far away from our destination
	  angleZ = angleZ - 1;
	}
    }

  glRotatef(angleZ, 0.0, 0.0, -10.0);
}

void Ship::hover()
{
  if (hoverY < 100)
    {
      hereIAm[1] = hereIAm[1]+0.0005;
      hoverY++;
    }
  else if ( (hoverY >= 100) && (hoverY < 200) )
    {
      hereIAm[1] = hereIAm[1]-0.0005;
      hoverY++;
    }
  else
    {
      hoverY = 0;
    }
}

void Ship::drawBody()
{
  glPushMatrix(); // Save matrix
  glPushAttrib(GL_CURRENT_BIT); // Save color

  // Scale
  glScalef(0.7,0.3,1.0); //Size of cuboid

  /* Draw Cuboid "body" */
  glColor3f(1,0,0);
  //glutSolidCube(1);
  glutSolidSphere (0.6, // radius
		   20, // slices
		   20 // stacks
		   );

  glPopMatrix(); // Restore matrix
  glPopAttrib(); // Restore color
}

void Ship::drawWindshield()
{
  glPushMatrix(); // Save matrix
  glPushAttrib(GL_CURRENT_BIT); // Save color


  /* Draw Sphere "windshield" */
  glColor3f(0,1,0);
  glTranslatef(0.0,0.0,-0.25);
  glutSolidSphere(0.27, // Radius
		  10,  // slices 
		  10  // stacks
		  );
  

  glPopMatrix(); // Restore matrix
  glPopAttrib(); // Restore color

}
