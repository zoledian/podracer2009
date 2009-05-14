#include <iostream>
#include "ship.h"
#include "math.h"
#include "camera.h"
#include <GL/glut.h>
using namespace std;

Ship::Ship()
{
  hereIAm[0] =  hereIAm[1] = hereIAm[2] = jumpDestination = jumpHeight = 0.0;
  
  moving = turning = jumping = false;

  cube = 0; // 0 = middle -2 = leftmost 2 = rightmost
  angleZ = hoverY = angleX = 0;
}

void Ship::drawShip(Camera* Cam)
{
  glMatrixMode(GL_MODELVIEW);

  // Translate
  glTranslatef(hereIAm[0],hereIAm[1],hereIAm[2]);

  // Rotate
  if(turning)
    turn();
  else
    hover();

  if(jumping)
    jump();

  // Draw
  drawBody();
  drawWindshield();

  // Point camera on ship
  Cam->LookAtThis(hereIAm[0],hereIAm[1]+jumpHeight,hereIAm[2]);

  // Move a bit forward the next time
  if (moving)
    hereIAm[2] = hereIAm[2] - 0.050;

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

void Ship::jumpShip()
{
  if (moving == false)
    {
      moving = true;
      return;
    }
  else if (jumping == false)
    {
      jumpDestination = hereIAm[2] - 4;
      jumping = true;
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

void Ship::jump()
{
  
  // Distance we have left
  GLfloat distanceLeft = hereIAm[2] - jumpDestination;
  // Remove negative distance values
  distanceLeft = fabs(distanceLeft);
  // Make floats behave!
  if (distanceLeft < 0.01)
    distanceLeft = 0.000;
  
  // Adjust X-axis angle
  if (distanceLeft > 3 && angleX < 40)
    {
      angleX++;
      angleX++;
      jumpHeight = jumpHeight + 0.025;
    }
  else if (distanceLeft > 2 && distanceLeft < 3 && angleX > 0)
    {
      angleX--;
      angleX--;
      jumpHeight = jumpHeight + 0.025;
    }
  else if (distanceLeft > 0.75 && distanceLeft < 2 && angleX > -40)
    {
      angleX--;
      angleX--;
      jumpHeight = jumpHeight - 0.025;
    }
  else if (distanceLeft > 0 && distanceLeft < 0.75 && angleX < 0)
    {
      angleX = angleX + 3;
      if (angleX > 0)
	angleX = 0;
      jumpHeight = jumpHeight - 0.025;
    }
  //cout << angleX << endl;
    
    // Are we done with our jump?
    if (distanceLeft == 0.0)
      {
	jumping = false;
	jumpHeight = 0.0;
	angleX = 0;
      }
    
    // Rotate
    glRotatef(angleX, 10.0, 0.0, 0.0);
    
    // Jump
    glTranslatef(0.0,jumpHeight,0.0);
  
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
