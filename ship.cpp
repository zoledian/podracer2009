#include <iostream>
#include "ship.h"
#include "math.h"
#include "camera.h"
#include <GL/glut.h>
#include <string.h>
#include <sstream>
#include "readjpeg.h"
#include "stdlib.h"
#include "particlesystem.h"
using namespace std;

Ship::Ship(Camera* cam)
{ 
  /**
   ** Create parts that will form the ship
   */
  _shipBody = gluNewQuadric();
  gluQuadricDrawStyle(_shipBody, GLU_FILL);
  gluQuadricNormals(_shipBody, GLU_SMOOTH);
  gluQuadricTexture(_shipBody, GL_TRUE);

  _shipWindshield = gluNewQuadric();
  gluQuadricDrawStyle(_shipWindshield, /*GLU_SILHOUETTE*/ GLU_FILL);
  gluQuadricNormals(_shipWindshield, GLU_SMOOTH);
  gluQuadricTexture(_shipWindshield, GL_TRUE);

  _shipEngine = gluNewQuadric();
  gluQuadricDrawStyle(_shipEngine, GLU_FILL);
  gluQuadricNormals(_shipEngine, GLU_SMOOTH);
  gluQuadricTexture(_shipEngine, GL_TRUE);

  _shipEngineBack = gluNewQuadric();
  gluQuadricDrawStyle(_shipEngineBack, GLU_FILL);
  gluQuadricNormals(_shipEngineBack, GLU_SMOOTH);
  gluQuadricTexture(_shipEngineBack, GL_TRUE);

  /**
   ** "Physics" variables
   */
  hoverHeight = 0.0025; // How high/low to hover
  velocity = 0.050;     // Speed of ship
  turnSpeed = 0.050;    // Speed of switching to next cube
                        // * must be abled to divide with 1! *
  jumpLength = 85.0;   // How far (times velocity) to jump
                        // (100 -> 5.0 steps if vel = 0.050)

  /**
   ** Helper variables
   **/
  _location[0] =  _location[1] =  _location[2] = 0.0;
  _locationOfFront[0] =  _locationOfFront[1] = 0.0;
  _locationOfFront[2] = -0.5;

  _jumpDestinationZ = _wiggleX = _shipAngleX = 0.0;

  _moving = _turning = _jumping = _wiggle = _wiggleToRight
    = _falling = false;

  _cubeNr = _wiggleCounter = _turnAngle = _hoverY = _jumpAngleX = _hoverCounter 
    = _wiggleAngle = 0;

  _currentSpeed = 0.001;
  _camera = cam;
  //_camera->reset();
  _camera->LookAtThis(_location[0],_location[1],_location[2]-80);
  _camera->slowZ = true;
  _camera->still = false;

  textureId = loadTexture("./textures/ship.jpg");
  textureId2 = loadTexture("./textures/windshield.jpg");

  hereWeDie = -10.0;

  _jumpOrigin = 0.0;

  _jumpLengthOriginal = jumpLength;

  _shipFumes = 1.0;

  readyForNextLevel = false;
  _warping = false;
  _warpCounter = 0.0;
  _intro = true;
  _highscore = 0;


  // Particlesystem for engine
  flame1 = new ParticleSystem();
  flame2 = new ParticleSystem();
}

void Ship::display(GLdouble blockDistance, GLdouble blockAngle, int blockType)
{
  printHighscore();

  glMatrixMode(GL_MODELVIEW);

  if ( _warping == false && _intro == false)
    {
      /* First: Change internal variables, do not rotate or translate yet */
      if (blockType == 4)
	{
	  _warping = true;
	}
      else if (blockType == 3)
	{
	  jumpLength = _jumpLengthOriginal + (_jumpLengthOriginal*0.2);
	}
      else
	jumpLength = _jumpLengthOriginal;

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

      glPushMatrix(); // Save matrix
      glPushAttrib(GL_CURRENT_BIT); // Save color

      /* Translate */
      glTranslatef(_location[0]+_wiggleX,
		   _location[1]+_hoverY,
		   _location[2]);

      /* ALL rotations */
      glRotatef((_shipAngleX+_jumpAngleX), 10.0, 0.0, 0.0);
      glRotatef(_turnAngle, 0.0, 0.0, -10.0);
      glRotatef((_turnAngle + _wiggleAngle), 0.0, -10.0, 0.0);

      drawShip();

      glPopMatrix(); // Restore matrix
      glPopAttrib(); // Restore color

      /* Point camera on ship */
      _camera->LookAtThis(_location[0],_location[1],_location[2]);

      /* Change variables so next time we move forward (and accelerate) */
      if (_moving)
	{  
	  if (_currentSpeed < velocity
	      && !_jumping)
	    {
	      _currentSpeed = _currentSpeed + 0.001;
	    }

	  // Move forward & up
	  _location[1] = _location[1] + (_currentSpeed * tan(blockAngle * (M_PI/180)));
	  _location[2] = _location[2] - _currentSpeed; // Speed is constant

	  if (_shipFumes < 0.5)
	    _shipFumes += 0.01;

	  _highscore += 0.01;
	}
    }
  else if (_warping == true)
    warp();
  else if (_intro == true)
    intro();

}

void Ship::drawShip()
{
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  /* Draw ship */
  drawBody();
  drawEngine(1);
  drawEngine(2);
  drawWindshield();
}

void Ship::moveHere(GLint cubeNr)
{
  // Keep ship from falling down on the sides
  if (fabs(_cubeNr+cubeNr) < 3)
    {
      _cubeNr = _cubeNr+cubeNr;
      _turning = true;

      if (_highscore > 0)
	_highscore -= 0.5;
    }
}

void Ship::jumpShip()
{
  if (_moving == false)
    {
      _moving = true;
      _location[0] = _location[1] = _location[2] = 0.00;
      return;
    }
  else if (_jumping == false && _falling == false)
    {
      _jumpOrigin = _location[2];
      _jumpDestinationZ = _location[2] - (jumpLength * _currentSpeed);
      _jumpDestinationZ = (jumpLength * _currentSpeed);
      _jumping = true;
      _shipFumes = 1.0;

      if (_highscore > 0)
	_highscore -= 0.5;
    }
}

GLdouble* Ship::getPosition()
{
  _locationOfFront[0] = _location[0];
  _locationOfFront[1] = _location[1];
  _locationOfFront[2] = _location[2];

  return _locationOfFront;
}

/**
 ** INTERNAL FUNCTIONS
 */

void Ship::gravity(GLdouble blockDistance, GLdouble blockAngle)
{
  _falling = false;

  if (blockDistance != 0.0) // Update die variable
    hereWeDie = blockDistance - 20.0;

  if (((blockDistance >= 0.7) || (blockDistance == 0)) // Are we falling down?
      && (!_jumping)   
      /*&& (blockAngle == 0)*/)
    {
      _location[1] = _location[1] - 0.10;
      _falling = true;
      if (_jumpAngleX > -50)
	_jumpAngleX--;
    }
  else if (blockDistance < 0.5) // Keep ship near block!
    {
	 _location[1] += 0.01;
      if ((_jumpAngleX < 0) && !_jumping)
	_jumpAngleX += 2;
    }
  else if ((blockDistance) > 0.5 
	   && (blockDistance < 0.7)
	   && (!_jumping))
    {
      if (_jumpAngleX < 0)
	_jumpAngleX += 2;
    }

  if (blockDistance >= -0.15   // Fixes a problem where you suddenly die
      && blockDistance < 0.00) // for no reason. Should probably fix 
    _location[1] += 0.15;      // this proberly
  

  // Are we dead?
  if ( (blockDistance < -0.15))
    {
      _camera->slowZ = true;
      new (this)Ship(_camera);
    }
  else if (_location[1] < hereWeDie)
    {
      _camera->slowZ = true;
      new (this)Ship(_camera);
    }
}

void Ship::jump()
{
  GLint jumpAngleMax = 30;

  // Calculate the relative distance we have left in our jump
  GLdouble distanceFactor = 
    fabs((_location[2] - _jumpOrigin) / _jumpDestinationZ);
  GLdouble jumpHeight;

  if (distanceFactor <= 0.50)
    {
      jumpHeight = (0.10 - (_currentSpeed * sin(distanceFactor * M_PI)))/2;
      _location[1] = _location[1] + jumpHeight;

      if (_jumpAngleX < jumpAngleMax)
	_jumpAngleX += 3;

    }
  else if ((distanceFactor <= 0.99)
	   && (distanceFactor > 0.50))
    {
      jumpHeight = (_currentSpeed * (1 - sin(distanceFactor * M_PI)))*2.0;

      _location[1] = _location[1] - jumpHeight;

      if ((_jumpAngleX > 0)
	  && (distanceFactor > 0.800))	  
	{
	_jumpAngleX -= 2;

	if (_jumpAngleX < 0)
	  _jumpAngleX = 0;

	if (_shipFumes > 0.5)
	  _shipFumes -= 0.05;
	}
    }
  else
    {
      _jumping = false;
      _jumpAngleX = 0;
      _location[1] -= 0.08;
    } 
}

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

void Ship::warp()
{
  glPushMatrix(); // Save matrix
  glPushAttrib(GL_CURRENT_BIT); // Save color

  _warpCounter += 0.1;
  _shipFumes = 1.0;

  glTranslatef(_location[0]+_wiggleX,
	       _location[1]+_hoverY+_warpCounter,
	       _location[2]-_warpCounter*10);

  drawShip();

  glPopMatrix(); // Restore matrix
  glPopAttrib(); // Restore color

  _camera->LookAtThis(_location[0],
		      _location[1], 
		      (_location[2] - (_warpCounter*0.2)));

  if (_warpCounter >= 10)
    printLevelComplete();

  if (_warpCounter >= 30)
    {
      readyForNextLevel = true;
      _camera->reset();
    }
 
}

void Ship::intro()
{
  if(_location[2] > 0)
    {
      if((_location[2] <= 30) && (_location[2] > 20))
	  _location[2] = _location[2] - 0.5;
      else if((_location[2] <= 20) && (_location[2] > 5))
	{
	  _camera->still = false;
	  _location[2] = _location[2] - 0.5;
	}
      else if (_location[2] <= 5)
	{
	  _location[2] = _location[2] - 0.05;
	  _shipFumes -= 0.02;
	}
      else
	  _location[2] = _location[2] - 0.5;

      hover();

    }
  else
    {
      _shipFumes = 0.00;
      _intro = false;
    }

  glPushMatrix(); // Save matrix
  glPushAttrib(GL_CURRENT_BIT); // Save color
  
  glTranslatef(_location[0],
	       _location[1]+_hoverY,
	       _location[2]);
  
  drawShip();
  
  glPopMatrix(); // Restore matrix
  glPopAttrib(); // Restore color
  
  _camera->LookAtThis(_location[0],_location[1],_location[2]);

}

void Ship::drawBody()
{
  glPushMatrix(); // Save matrix

  // Scale
  glScalef(0.5,0.3,1.0); //Size of squashed sphere

  glRotatef(180, 0.0, 0.15, 10.0);

  // Specify texture to use
  glBindTexture(GL_TEXTURE_2D, textureId);

  // Create GLU sphere
  gluSphere(_shipBody, 0.6, 16, 16);

  glPopMatrix(); // Restore matrix
}

void Ship::drawEngine(GLint nr)
{
  double translateX;
  if(nr == 1)
    translateX = 0.25;
  else
    translateX = -0.25;

  glPushMatrix(); // Save matrix

  /* Draw cyliner */
  glTranslatef(translateX,
	       -0.04,
	       -0.1
	       );
  glBindTexture(GL_TEXTURE_2D, textureId);
  glRotatef(180, 0, 0, 10.0);
  gluCylinder(_shipEngine,
	      0.12, // base
	      0.27, // top
	      0.7, // height
	      12, //slices
	      12  // stacks
	      );

  /* Draw bottom disk "fumes" */
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glPushAttrib(GL_CURRENT_BIT); // Save color

  glColor3f(_shipFumes,0.5*_shipFumes,0);
  glTranslatef(0.0,
	       0.0,
	       0.7
	       );
  gluDisk(_shipEngineBack,
	  0, // inner radius
	  0.27, // outer radius
	  12, // slices
	  12 // rings
	  );
  
  // Draw engine "flames"
  
  if(_moving == true)
    {
      // Activate flames if they are disabled
      if(flame1->active == false || flame2->active == false)
	{
	  flame1->activate();
	  flame2->activate();
	}  
      // Draw flame corresponding to the engine currently being drawn
      if(nr == 1)
	flame1->draw();
      else
	flame2->draw();
    }
  else
    {
      // Disable flames if they are disabled
      if(flame1->active == true || flame2->active == true)
	{
	  flame1->disable();
	  flame2->disable();
	}
    }
    


  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_TEXTURE_2D);
  glPopAttrib(); // Restore color
  
  /* Draw top disk */
  glBindTexture(GL_TEXTURE_2D, textureId);
  glRotatef(180, 0, 10, 0.0);
  glColor3f(0.3,0.3,0.3);
  glTranslatef(0.0,
	       0.0,
	       0.7// back
	       );
  gluDisk(_shipEngineBack,
	  0, // inner radius
	  0.12, // outer radius
	  12, // slices
	  12 // rings
	  );

  glPopMatrix(); // Restore matrix
}

void Ship::drawWindshield()
{
  glPushMatrix(); // Save matrix

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_SRC_COLOR);

  GLfloat mat_shininess[] = { 100.0 };
  GLfloat mat_ambient[] = { 10, 1, 1, 0.0 };
  GLfloat mat_diffuseColor[] = { 10, 0.1, 0.1, 0.0 };
  GLfloat mat_specularColor[] = { 10, 10, 10, 0.0 };
  
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseColor);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specularColor);
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

  glBindTexture(GL_TEXTURE_2D, textureId2);

  double clip_plane1[]={0.0, 1.0, 0.0, 0};
  glClipPlane(GL_CLIP_PLANE1,clip_plane1);
  glEnable(GL_CLIP_PLANE1);

  // Draw Sphere "windshield"
  glTranslatef(0.0,0.0,-0.20);

  gluSphere(_shipWindshield, 0.27, 16, 16);

  //disable clip plane
  glDisable(GL_CLIP_PLANE1);

  glDisable(GL_BLEND);

  glPopMatrix(); // Restore matrix
}

void Ship::printHighscore()
{
  glPushMatrix(); // Save matrix
  glPushAttrib(GL_CURRENT_BIT); // Save color

  string text = "Highscore: ";
  
  std::stringstream ss;
  ss << "Highscore: " << (GLint) _highscore;
  std::string s( ss.str() );

  for ( unsigned int i = 0; i < s.length(); i++)
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);

  glPopMatrix(); // Restore matrix
  glPopAttrib(); // Restore color

}

void Ship::printLevelComplete()
{
  glPushMatrix(); // Save matrix
  glPushAttrib(GL_CURRENT_BIT); // Save color

  GLdouble* camLocation = _camera->getLocation();

  glRasterPos3f(camLocation[0] -0.5,
		camLocation[1] -1,
		camLocation[2] -1);

  string s = "Level Complete";
  
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

  if (!pixelData)
    exit(0);

  glGenTextures(1, &texNum);
  glBindTexture(GL_TEXTURE_2D, texNum);
  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixelData);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  free(pixelData);

  return texNum;
}

void Ship::reset()
{
  GLdouble highscore = _highscore;

  new (this)Ship(_camera);

  _location[0] =  _location[1] = 0.0;
  _location[2] = 70.0;
  _camera->slowZ = true;
  _camera->still = true;
  _camera->reset();

  _highscore = highscore;
}
