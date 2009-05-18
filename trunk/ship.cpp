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
  jumpLength = 80.0;   // How far (times velocity) to jump
                        // (100 -> 5.0 steps if vel = 0.050)

  /**
   ** Helper variables
   **/
  _location[0] =  _location[1] = _location[2] = 0.0;
  _locationOfFront[0] =  _locationOfFront[1] = 0.0;
  _locationOfFront[2] = -0.5;

  _jumpDestinationZ = 0.0;
  _wiggleX = _shipAngleX = 0.0;

  _moving = _turning = _jumping = _wiggle = _wiggleToRight
    = _falling = false;

  _cubeNr = _wiggleCounter = _turnAngle = _hoverY = _jumpAngleX = _hoverCounter 
    = _wiggleAngle = 0;

  _currentSpeed = 0.001;

  _camera = cam;
  _camera->LookAtThis(0.0,0.0,-100.0);
  _camera->slowZ = true;

  textureId = loadTexture("./textures/ship3.jpg");

  hereWeDie = -10.0;

  _jumpOrigin = 0.0;

}

void Ship::drawShip(GLdouble blockDistance, GLdouble blockAngle)
{
  printHighscore();

  glMatrixMode(GL_MODELVIEW);

  /* First: Change internal variables, do not rotate or translate yet */

  // Adjust ship X angle to block angle
  if (_shipAngleX > blockAngle)
    _shipAngleX -= 0.5;
  else if (_shipAngleX < blockAngle)
    _shipAngleX += 0.5;
 
  // "Gravity"
  gravity(blockDistance, blockAngle);

  // Jump
  if(_jumping)
    jump();

  // Turn, wiggle or hover
  if(_turning)
    turn();
  else if (_wiggle)
    wiggle();
  else
    hover();

  /* Translate */
  glTranslatef(_location[0]+_wiggleX,
	       _location[1]+_hoverY,
	       _location[2]);

  /* ALL rotations */
  glRotatef((_shipAngleX+_jumpAngleX), 10.0, 0.0, 0.0);
  glRotatef(_turnAngle, 0.0, 0.0, -10.0);
  glRotatef((_turnAngle + _wiggleAngle), 0.0, -10.0, 0.0);

  /* Draw ship */
  drawBody();
  drawWindshield();

  /* Point camera on ship */
  _camera->LookAtThis(_location[0],_location[1],_location[2]);

  /* Change variables so next time we move forward (and accelerate) */
  if (_moving)
    {  
      if (_currentSpeed < velocity)
	{
	  _currentSpeed = _currentSpeed + 0.001;
	}

      // Move forward
      // _location[2] = _location[2] - _currentSpeed; OLD
      _location[1] = _location[1] + (_currentSpeed * tan(blockAngle * (M_PI/180)));

      _location[2] = _location[2] - _currentSpeed; // Speed is constant
      //_location[2] = _location[2] - (_currentSpeed * cos(blockAngle*(M_PI/180))); // Speed is dependant on angle
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
      _jumpOrigin = _location[2];
      _jumpDestinationZ = _location[2] - (jumpLength * _currentSpeed);
      _jumpDestinationZ = (jumpLength * _currentSpeed);
      _jumping = true;
    }
}

GLdouble* Ship::getPosition()
{
  _locationOfFront[0] = _location[0];
  _locationOfFront[1] = _location[1];
  _locationOfFront[2] = _location[2];// - 0.5;

  return _locationOfFront;
}

/**
 ** INTERNAL FUNCTIONS
 */

void Ship::gravity(GLdouble blockDistance, GLdouble blockAngle)
{
  _falling = false;

  // Update die variable
  if (blockDistance != 0.0)
    hereWeDie = blockDistance - 20.0;

  // Are we falling down?
  if (((blockDistance >= 0.7) || (blockDistance == 0)) 
      && (!_jumping) 
      && (blockAngle == 0))
    {
      _location[1] = _location[1] - 0.10;
      _falling = true;
      if (_jumpAngleX > -50)
	_jumpAngleX--;
    }
  // Keep ship near block!
  else if (blockDistance < 0.5)
    {
      _location[1] += 0.01;
      if ((_jumpAngleX < 0) && !_jumping)
	_jumpAngleX += 2;
    }
  else if ((blockDistance) > 0.5 
	   && (blockDistance < 0.7)
	   && (!_jumping))
    {
      _location[1] -= 0.01;
      if (_jumpAngleX < 0)
	_jumpAngleX += 2;
    }
  /*
  cout << "Ship y: " << _location[1] << endl;
  cout << "blockDistance: " << blockDistance << endl;
  cout << "Here we die: " << hereWeDie << endl;
  */

  // Are we dead?
  if ( (blockDistance < -0.10))
    {
      cout << "You died because: blockDistance = " << blockDistance << endl;
      _camera->slowZ = true;
      new (this)Ship(_camera);
    }
  else if (_location[1] < hereWeDie)
    {
      cout << "You died because: You fell down." << endl;
      _camera->slowZ = true;
      new (this)Ship(_camera);
    }
  else if (blockDistance > -0.10
	   && blockDistance < 0.00)
    _location[1] += 0.10;

}

/* Old gravity
void Ship::gravity(GLdouble yDistance)
{
  _falling = false;

  if (yDistance < 0.05 && yDistance > -0.05)
    yDistance = 0.0;

  if (yDistance != 0.0)
    hereWeDie = yDistance - 20.0;

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
    
      _camera->slowZ = true;
      new (this)Ship(_camera);

    }
} END OF OLD GRAVITY */

void Ship::jump()
{
  GLint jumpAngleMax = 30;

  // Calculate the relative distance we have left in our jump
  GLdouble distanceFactor = fabs((_location[2] - _jumpOrigin) / _jumpDestinationZ);
  GLdouble jumpHeight;

  if (distanceFactor <= 0.500)
    {
      //jumpHeight = _currentSpeed - (_currentSpeed * sin(distanceFactor * M_PI));
      jumpHeight = (0.10 - (_currentSpeed * sin(distanceFactor * M_PI)))/2;
      _location[1] = _location[1] + jumpHeight;

      if (_jumpAngleX < jumpAngleMax)
	_jumpAngleX += 3;

    }
  else if ((distanceFactor <= 0.999)
	   && (distanceFactor > 0.500))
    {
      jumpHeight = (_currentSpeed * (1 - sin(distanceFactor * M_PI)))*1.8;

      _location[1] = _location[1] - jumpHeight;

      if ((_jumpAngleX > 0)
	  && (distanceFactor > 0.800))	  
	{
	_jumpAngleX -= 2;
	}

     /*
      else if ((_jumpAngleX > (-jumpAngleMax))
	  && (distanceFactor >= 0.800)
	  && (distanceFactor < 0.900))
	{
	  _jumpAngleX -= 1;
	}
      else if ((_jumpAngleX < 0)
	       && (distanceFactor >= 0.900))
	{
	  _jumpAngleX += 1;
	  if(_jumpAngleX > 0)
	    _jumpAngleX = 0;

	  cout << _jumpAngleX << endl;
	}
      */

    }
  else
    {
      _jumping = false;
      _jumpAngleX = 0;
      _location[1] -= 0.10;
    } 
}

  /* OLD JUMP
void Ship::jump()
{
  GLint jumpAngleMax = 25;

  // Calculate the distance we have left in our jump
  GLfloat distanceLeft = _location[2] - _jumpDestinationZ;

  // Make floats behave!
  if (distanceLeft < 0.01)
    distanceLeft = 0.000;
  
  // Adjust X-axis angle, (jumpAngleX)
  // & Ship's y location    (_location[1])
  if ( (distanceLeft > (0.75*jumpLength*_currentSpeed)))
    {
      _location[1] = _location[1] + 0.06;

      if (_jumpAngleX < jumpAngleMax)
	_jumpAngleX += 1;
    }
  else if ( (distanceLeft > ( 0.50*jumpLength*_currentSpeed ))
	    && (distanceLeft <= ( 0.75*jumpLength*_currentSpeed )))
    {
      _location[1] = _location[1] + 0.04;

      if (_jumpAngleX < jumpAngleMax)
	_jumpAngleX += 1;
    }
  else if ( (distanceLeft > (0.25*jumpLength*_currentSpeed))
	    && (distanceLeft <= (0.50*jumpLength*_currentSpeed)))
    {
      _location[1] = _location[1] - 0.04;
    }
    else if ( (distanceLeft > 0.00 )
	      && ( distanceLeft <= (0.25*jumpLength*_currentSpeed) ))
    {
      if (_jumpAngleX > 0)
	_jumpAngleX -= 1;

      _location[1] = _location[1] - 0.06;
    }
    
    // Are we done with our jump?
    if (distanceLeft == 0.0 || distanceLeft < 0.0)
      {
	_jumping = false;
	_jumpAngleX = 0;
      } 
}
  */

void Ship::turn()
{
  // Calculate distance we have left in our turn
  GLfloat distanceLeft = _location[0] - _cubeNr;

  // Are we moving left or right?
  GLboolean movingLeft = true;
  if (distanceLeft < 0)
    { movingLeft = false; }

  // Remove negative distance values
  distanceLeft = fabs(distanceLeft);

  // Rotate when moving
  turnRotateZ(movingLeft, distanceLeft);

  // Move by the x axis towards our destination
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
      // We are moving right
      if ( (distanceLeft < 0.5) && (_turnAngle > 0) )
	{
	  // We are close to our destination
	  _turnAngle--;
	}
      else if (_turnAngle < 30)
	{
	  // We are far away from our destination
	  _turnAngle++;
	}
    }
  else if (movingLeft)
    {
      // We are moving left
      if ( (distanceLeft < 0.5) && (_turnAngle < 0) )
	{
	  // We are close to our destination
	  _turnAngle++;
	}
      else if (_turnAngle > -30)
	{
	  // We are far away from our destination
	  _turnAngle--;
	}
    }
}

void Ship::wiggle()
{
  _wiggleCounter++;

  GLint wiggleLength = 10;
  GLint wiggleBack = wiggleLength*2;

  if (_wiggleCounter < wiggleLength)
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
  else if ( (_wiggleCounter >= wiggleLength )
	    && (_wiggleCounter < wiggleBack) )
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
 
}

void Ship::hover()
{
  if (_hoverCounter < 100)
    {
      _hoverY = _hoverY + hoverHeight;
      _hoverCounter++;
    }
  else if ( (_hoverCounter >= 100) && (_hoverCounter < 200) )
    {
      _hoverY = _hoverY - hoverHeight;
      _hoverCounter++;
    }
  else
    {
      _hoverCounter = 0;
      _hoverY = 0;
    }
}

void Ship::drawBody()
{
  glPushMatrix(); // Save matrix
  glPushAttrib(GL_CURRENT_BIT); // Save color

  // Scale
  glScalef(0.7,0.3,1.0); //Size of squashed sphere

  // Texture stuff
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glEnable(GL_TEXTURE_GEN_R);

  // Draw Cuboid "body"
  glColor3f(0.5,0,0);
  glutSolidSphere (0.6, // radius
		   20, // slices
		   20 // stacks
		   );

  // Disable texture stuff
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

  // Draw Sphere "windshield"
  glColor3f(1,1,1);
  glTranslatef(0.0,0.0,-0.25);
  glutSolidSphere(0.27, // Radius
		  10,  // slices 
		  10  // stacks
		  );
  
  glPopMatrix(); // Restore matrix
  glPopAttrib(); // Restore color

}

void Ship::printHighscore()
{
  //glMatrixMode(GL_PROJECTION);
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
  /* We dont need PPM, less things to worry about

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
