#include <iostream>
#include "ship.h"
#include "math.h"
#include "camera.h"
#include <GL/glut.h>
using namespace std;

Ship::Ship()
{ 
  /**
   ** "Physics" variables
   */
  hoverHeight = 0.0025; // How high/low to hover
  velocity = 0.050;     // Speed of ship
  turnSpeed = 0.050;    // Speed of switching to next cube
                        // * must be abled to divide with 1! *
  jumpLength = 100.0;   // How far (times velocity) to jump
                        // (100 -> 5.0 steps if vel = 0.050)

  /**
   ** Helper variables
   **/
  hereIAm[0] =  hereIAm[1] = hereIAm[2] = 0;
  jumpDestination = jumpHeight = 0.0;
  turnEndX = 0.0;
  moving = turning = jumping = turningEnd = turningEndDirectionIsRight = false;
  currentSpeed = 0.001;
  cube = turnEndCounter = 0;
  turnAngle = hoverY = angleX = hoverCounter = turnEndAngle = 0;
}

void Ship::drawShip(Camera* Cam)
{
  glMatrixMode(GL_MODELVIEW);

  // Translate
  glTranslatef(hereIAm[0]+turnEndX,hereIAm[1]+hoverY,hereIAm[2]);

  // Rotate
  if(turning)
    turn();
  else if (turningEnd)
    turnEnd();
  else
    hover();

  if(jumping)
    jump();

  // Draw
  drawBody();
  drawWindshield();

  // Point camera on ship
  Cam->LookAtThis(hereIAm[0],hereIAm[1]+jumpHeight,hereIAm[2]);

  if (moving)
    {  
      if (currentSpeed <= velocity)
	{
	  currentSpeed = currentSpeed + 0.001;
	}
      
      // Move a bit forward the next time
      hereIAm[2] = hereIAm[2] - currentSpeed;
    }

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
      jumpDestination = hereIAm[2] - (jumpLength * currentSpeed);
      jumping = true;
    }
}

GLdouble* Ship::getPosition()
{
  return hereIAm;
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
    { hereIAm[0] = hereIAm[0]-turnSpeed; }
  else
    { hereIAm[0] = hereIAm[0]+turnSpeed; }

  // Make floats behave!
  if (distanceLeft < 0.01)
    distanceLeft = 0.000;

  // Have we arrived?
  if (distanceLeft == 0)
    {
      hereIAm[0] = cube;
      turnAngle = 0;
      turning = false;

      turningEnd = true;
      turningEndDirectionIsRight = (!movingLeft);
      turnEndAngle = 0;
      turnEndX = 0.0;
    }
}

void Ship::turnRotateZ(GLboolean movingLeft, GLfloat distanceLeft)
{
  if ( !movingLeft )
    {
      if ( (distanceLeft < 0.5) && (turnAngle > 0) )
	{
	  // We are moving right and are close to our destination
	  turnAngle--;
	}
      else if (turnAngle < 30)
	{
	  // We are moving right and are far away from our destination
	  turnAngle++;
	}
    }
  else if (movingLeft)
    {
      if ( (distanceLeft < 0.5) && (turnAngle < 0) )
	{
	  // We are moving left and are close to our destination
	  turnAngle++;
	}
      else if (turnAngle > -30)
	{
	  // We are moving left and are far away from our destination
	  turnAngle--;
	}
    }

  glRotatef(turnAngle, 0.0, 0.0, -10.0);
  glRotatef(turnAngle, 0.0, -10.0, 0.0);
}

void Ship::turnEnd()
{
  turnEndCounter++;
  GLint turnLength = 10;
  GLint turnBack = turnLength*2;

  if (turnEndCounter < turnLength)
    {
      if (turningEndDirectionIsRight)
	{
	  turnEndAngle++;
	  turnEndX = turnEndX - 0.001;
	}
      else
	{
	  turnEndAngle--;
	  turnEndX = turnEndX + 0.001;
	}
    }
  else if ( (turnEndCounter >= turnLength )
	    && (turnEndCounter < turnBack) )
    {
      if (turningEndDirectionIsRight)
	{
	  turnEndAngle--;
	  turnEndX = turnEndX + 0.001;
	}
      else
	{
	  turnEndAngle++;
	  turnEndX = turnEndX - 0.001;
	}
    }
  else
    {
      turnEndAngle = 0;
      turnEndX = 0.0;
      turningEnd = false;
      turnEndCounter = 0;
    }
  
  glRotatef(turnEndAngle, 0.0, -10.0, 0.0);
}

void Ship::hover()
{
  if (hoverCounter < 100)
    {
      hoverY = hoverY+hoverHeight;
      hoverCounter++;
    }
  else if ( (hoverCounter >= 100) && (hoverCounter < 200) )
    {
      hoverY = hoverY-hoverHeight;
      hoverCounter++;
    }
  else
    {
      hoverCounter = 0;
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
  if (distanceLeft > (0.75*jumpLength*currentSpeed) && angleX < 40)
    {
      angleX++;
      angleX++;
      jumpHeight = jumpHeight + 0.025;
    }
  else if (distanceLeft > (0.50*jumpLength*currentSpeed) 
	   && distanceLeft < (0.75*jumpLength*currentSpeed) && angleX > 0)
    {
      angleX--;
      angleX--;
      jumpHeight = jumpHeight + 0.025;
    }
  else if (distanceLeft > (0.25*jumpLength*currentSpeed) 
	   && distanceLeft < (0.50*jumpLength*currentSpeed) && angleX > -40)
    {
      angleX--;
      angleX--;
      jumpHeight = jumpHeight - 0.025;
    }
  else if (distanceLeft > 0.00 
	   && distanceLeft < (0.25*jumpLength*currentSpeed) && angleX < 0)
    {
      angleX = angleX + 2;
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
