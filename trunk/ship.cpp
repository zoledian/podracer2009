#include <iostream>
#include "ship.h"
#include "math.h"
#include "camera.h"
#include <GL/glut.h>
#include <string.h>
#include <sstream>
#include "readjpeg.h"
#include "stdlib.h"
using namespace std;

Ship::Ship(Camera* cam)
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
  _location[0] =  _location[1] = _location[2] = 0.0;
  jumpDestination = jumpHeight = 0.0;
  _wiggleX = _angle = 0.0;

  _moving = _turning = _jumping = _wiggle = _wiggleToRight
    = _falling = false;

  _cubeNr = _wiggleCounter = _turnAngle = _hoverY = angleX = _hoverCounter 
    = _wiggleAngle = 0;

  currentSpeed = 0.001;

  _camera = cam;
  _camera->LookAtThis(0.0,0.0,-100.0);
  _camera->slowZ = true;

  textureId = loadTexture("./textures/ship3.jpg");

  hereWeDie = -10.0;
}

void Ship::drawShip(GLdouble yDistance, GLdouble angle)
{
  printHighscore();

  glMatrixMode(GL_MODELVIEW);

  // "Gravity"
  gravity(yDistance, angle);

  // Translate
  glTranslatef(_location[0]+_wiggleX,
	       _location[1]+_hoverY,
	       _location[2]);

  // Jump
  if(_jumping)
    jump();

  // Compensate for block angle
  glRotatef(_angle, 10.0, 0.0, 0.0);

  // Turn, wiggle or hover
  if(_turning)
    turn();
  else if (_wiggle)
    wiggle();
  else
    hover();

  // Draw ship
  drawBody();
  drawWindshield();

  // Point camera on ship
  _camera->LookAtThis(_location[0],_location[1],_location[2]);

  // Move forward next time & accelerate
  if (_moving)
    {  
      if (currentSpeed <= velocity)
	{
	  currentSpeed = currentSpeed + 0.001;
	}
      
      _location[2] = _location[2] - currentSpeed;

      //printDebug();
    }

}

void Ship::moveHere(GLint cubeNr)
{
  // Keep ship from falling down on the sides
  if (fabs(_cubeNr+cubeNr) < 3)
    {
      _cubeNr = _cubeNr+cubeNr;
      _turning = true;
    }
}

void Ship::jumpShip()
{
  if (_moving == false)
    {
      _moving = true;
      return;
    }
  else if (_jumping == false && _falling == false)
    {
      jumpDestination = _location[2] - (jumpLength * currentSpeed);
      _jumping = true;
    }
}

GLdouble* Ship::getPosition()
{
  return _location;
}

void Ship::turn()
{
  // Distance we have left
  GLfloat distanceLeft = _location[0] - _cubeNr;

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
    { _location[0] = _location[0]-turnSpeed; }
  else
    { _location[0] = _location[0]+turnSpeed; }

  // Make floats behave!
  if (distanceLeft < 0.01)
    distanceLeft = 0.000;

  // Have we arrived?
  if (distanceLeft == 0)
    {
      _location[0] = _cubeNr;
      _turnAngle = 0;
      _turning = false;

      _wiggle = true;
      _wiggleToRight = (!movingLeft);
      _wiggleAngle = 0;
      _wiggleX = 0.0;
    }
}

void Ship::turnRotateZ(GLboolean movingLeft, GLdouble distanceLeft)
{
  if ( !movingLeft )
    {
      if ( (distanceLeft < 0.5) && (_turnAngle > 0) )
	{
	  // We are moving right and are close to our destination
	  _turnAngle--;
	}
      else if (_turnAngle < 30)
	{
	  // We are moving right and are far away from our destination
	  _turnAngle++;
	}
    }
  else if (movingLeft)
    {
      if ( (distanceLeft < 0.5) && (_turnAngle < 0) )
	{
	  // We are moving left and are close to our destination
	  _turnAngle++;
	}
      else if (_turnAngle > -30)
	{
	  // We are moving left and are far away from our destination
	  _turnAngle--;
	}
    }

  glRotatef(_turnAngle, 0.0, 0.0, -10.0);
  glRotatef(_turnAngle, 0.0, -10.0, 0.0);
}

void Ship::wiggle()
{
  _wiggleCounter++;
  GLint turnLength = 10;
  GLint turnBack = turnLength*2;

  if (_wiggleCounter < turnLength)
    {
      if (_wiggleToRight)
	{
	  _wiggleAngle++;
	  _wiggleX = _wiggleX - 0.001;
	}
      else
	{
	  _wiggleAngle--;
	  _wiggleX = _wiggleX + 0.001;
	}
    }
  else if ( (_wiggleCounter >= turnLength )
	    && (_wiggleCounter < turnBack) )
    {
      if (_wiggleToRight)
	{
	  _wiggleAngle--;
	  _wiggleX = _wiggleX + 0.001;
	}
      else
	{
	  _wiggleAngle++;
	  _wiggleX = _wiggleX - 0.001;
	}
    }
  else
    {
      _wiggleAngle = 0;
      _wiggleX = 0.0;
      _wiggle = false;
      _wiggleCounter = 0;
    }
  
  glRotatef(_wiggleAngle, 0.0, -10.0, 0.0);
}

void Ship::hover()
{
  if (_hoverCounter < 100)
    {
      _hoverY = _hoverY+hoverHeight;
      _hoverCounter++;
    }
  else if ( (_hoverCounter >= 100) && (_hoverCounter < 200) )
    {
      _hoverY = _hoverY-hoverHeight;
      _hoverCounter++;
    }
  else
    {
      _hoverCounter = 0;
      _hoverY = 0;
    }

}

void Ship::jump()
{
  GLint maxAngle = 25;

  // Distance we have left
  GLfloat distanceLeft = _location[2] - jumpDestination;

  // Make floats behave!
  if (distanceLeft < 0.01)
    distanceLeft = 0.000;
  
  // Adjust X-axis angle
  if ( (distanceLeft > (0.75*jumpLength*currentSpeed)))
    {
      jumpHeight = jumpHeight + 0.06;
      _location[1] = _location[1] + 0.06;

      if (angleX < maxAngle)
	angleX += 1;
    }
  else if ( (distanceLeft > ( 0.50*jumpLength*currentSpeed ))
	    && (distanceLeft <= ( 0.75*jumpLength*currentSpeed )))
    {
      jumpHeight = jumpHeight + 0.04;
      _location[1] = _location[1] + 0.04;
      if (angleX < maxAngle)
	angleX += 1;
    }
  else if ( (distanceLeft > (0.25*jumpLength*currentSpeed))
	    && (distanceLeft <= (0.50*jumpLength*currentSpeed)))
    {
      jumpHeight = jumpHeight - 0.04;
      _location[1] = _location[1] - 0.04;
      /*
      if (angleX > 0)
	angleX -= 1;
      */
    }
    else if ( (distanceLeft > 0.00 )
	      && ( distanceLeft <= (0.25*jumpLength*currentSpeed) ))
    {
      if (angleX > 0)
	angleX -= 1;
      jumpHeight = jumpHeight - 0.06;
      _location[1] = _location[1] - 0.06;
    }
    
    // Are we done with our jump?
    if (distanceLeft == 0.0 || distanceLeft < 0.0)
      {
	_jumping = false;
	jumpHeight = 0.0;
	angleX = 0;
      }
    
    // Rotate
    glRotatef(angleX, 10.0, 0.0, 0.0);
    
    // Jump
    //glTranslatef(0.0,jumpHeight,0.0);
  
}


void Ship::drawBody()
{
  glPushMatrix(); // Save matrix
  glPushAttrib(GL_CURRENT_BIT); // Save color

  // Scale
  glScalef(0.7,0.3,1.0); //Size of cuboid

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureId);

 
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
 
  glEnable(GL_TEXTURE_GEN_R);

  /* Draw Cuboid "body" */
  glColor3f(0.5,0,0);
  //glutSolidCube(1);
  glutSolidSphere (0.6, // radius
		   20, // slices
		   20 // stacks
		   );

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);

  glPopMatrix(); // Restore matrix
  glPopAttrib(); // Restore color
}

void Ship::drawWindshield()
{
  glPushMatrix(); // Save matrix
  glPushAttrib(GL_CURRENT_BIT); // Save color


  /* Draw Sphere "windshield" */
  glColor3f(1,1,1);
  glTranslatef(0.0,0.0,-0.25);
  glutSolidSphere(0.27, // Radius
		  10,  // slices 
		  10  // stacks
		  );
  

  glPopMatrix(); // Restore matrix
  glPopAttrib(); // Restore color

}

void Ship::gravity(GLdouble yDistance, GLdouble angle)
{
  _falling = false;

  if (yDistance < 0.05 && yDistance > -0.05)
    yDistance = 0.0;

  cout << "Y distance: " << yDistance << endl;
  cout << "Y location: " << _location[1] << endl;
  cout << "Die at: " << hereWeDie << endl;
  cout << endl;

  if (yDistance != 0.0)
    hereWeDie = yDistance - 20.0;

  // Compensate for block y distance
  if (yDistance < 0.7
      && yDistance > 0.1)
    {
      _location[1] = _location[1] + 0.06;
    }
  else if ((yDistance > 1.3) && (!_jumping))
    {
      _location[1] = _location[1] - 0.10;
      _falling = true;
    }
  else if ((yDistance == 0.0) && (!_jumping))
    {
      _location[1] = _location[1] - 0.10;
      _falling = true;
    }
  
  if ((yDistance < 0.0) 
      || (_location[1] < hereWeDie))
    {
      // We are dead
      _camera->slowZ = true;
      new (this)Ship(_camera);
    }
  
  // Compensate for block angle
  if (angle > _angle)
    _angle++;
  else if (angle < _angle)
    _angle--;
  else
    _angle = angle;
  

}

void Ship::printDebug()
{
  cout << "jumping? " << _jumping << endl;
  cout << "turning? " << _turning << endl;
  cout << "turning end? " << _wiggle << endl;
}

void Ship::printHighscore()
{
  glMatrixMode(GL_PROJECTION);
  //glMatrixMode(GL_MODELVIEW);

  glPushMatrix(); // Save matrix
  glPushAttrib(GL_CURRENT_BIT); // Save color

  string text = "Highscore: ";
  
  // Skapa en stringstream
  std::stringstream ss;

  // Lägg in saker i den..
  ss << "Highscore: " << (GLint) fabs(_location[2]);

  // Konvertera till en string
  std::string s( ss.str() );

  for ( unsigned int i = 0; i < s.length(); i++)
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);

  glPopMatrix(); // Restore matrix
  glPopAttrib(); // Restore color

}

GLuint Ship::loadTexture(char* name)
{
   GLuint texNum;
  int width = 0, height = 0;
  char* pixelData = 0;
  int nameLen = strlen(name);

  if ((nameLen >= 4 && (!strcmp(name + nameLen - 4, ".jpg")
			|| !strcmp(name + nameLen - 4, ".JPG")))
      || (nameLen >= 5 && (!strcmp(name + nameLen - 5, ".jpeg")
			   || !strcmp(name + nameLen - 5, ".JPEG"))))
    {
      read_JPEG_file(name, &pixelData, &width, &height);
    }
  /*
  else if (nameLen >= 4 && (!strcmp(name + nameLen - 4, ".ppm")
			    || !strcmp(name + nameLen - 4, ".PPM")))
    {
      pixelData = readppm(name, &width, &height);
    }
  */

  if (!pixelData)
    exit(0);

  glGenTextures(1, &texNum);
  glBindTexture(GL_TEXTURE_2D, texNum);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0,
	       GL_RGB, GL_UNSIGNED_BYTE, pixelData);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  free(pixelData);

  return texNum;
}
