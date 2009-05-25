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
  // Create parts that will form the ship
  shipBody_ = gluNewQuadric();
  gluQuadricDrawStyle(shipBody_, GLU_FILL);
  gluQuadricNormals(shipBody_, GLU_SMOOTH);
  gluQuadricTexture(shipBody_, GL_TRUE);

  shipWindshield_ = gluNewQuadric();
  gluQuadricDrawStyle(shipWindshield_, /*GLU_SILHOUETTE*/ GLU_FILL);
  gluQuadricNormals(shipWindshield_, GLU_SMOOTH);
  gluQuadricTexture(shipWindshield_, GL_TRUE);

  shipEngine_ = gluNewQuadric();
  gluQuadricDrawStyle(shipEngine_, GLU_FILL);
  gluQuadricNormals(shipEngine_, GLU_SMOOTH);
  gluQuadricTexture(shipEngine_, GL_TRUE);

  shipEngineBack_ = gluNewQuadric();
  gluQuadricDrawStyle(shipEngineBack_, GLU_FILL);
  gluQuadricNormals(shipEngineBack_, GLU_SMOOTH);
  gluQuadricTexture(shipEngineBack_, GL_TRUE);

  // "Physics" variables
  hoverHeight_ = 0.0025; // How high/low to hover
  velocity_ = 0.050;     // Speed of ship
  turnSpeed_ = 0.050;    // Speed of switching to next cube
                        // * must be abled to divide with 1! *
  jumpLength_ = 85.0;   // How far (times velocity) to jump
                        // (100 -> 5.0 steps if vel = 0.050)

  // Particlesystem for engine
  if (flame1_ == NULL || flame2_ == NULL)
    {
      flame1_ = new ParticleSystem();
      flame2_ = new ParticleSystem();
    }
  
  // Init camera
  camera_ = cam;
  camera_->LookAtThis(location_[0],location_[1],location_[2]-80);
  camera_->slowZ = true;
  camera_->still = false;

  // Load textures
  textureId = loadTexture("./textures/ship.jpg");
  textureId2 = loadTexture("./textures/windshield.jpg");

  // Various helper variables
  location_[0] =  location_[1] =  location_[2] =
  jumpDestination_ = wiggleX_ = shipAngleX_ = warpCounter_ = 0.0;

  moving_ = turning_ = jumping_ = wiggling_ = wiggleToRight_
    = falling_ = readyForNextLevel = warping_ = false;

  cubeNr_ = wiggleCounter_ = turnAngle_ = hoverY_ = jumpAngleX_ = hoverCounter_ 
    = wiggleAngle_ = 0;

  currentSpeed_ = 0.001;

  hereWeDie_ = -10.0;

  jumpOrigin_ = 0.0;

  jumpLengthOriginal_ = jumpLength_;

  shipFumes_ = 1.0;

  intro_ = true;

  highscore_ = 0;
}

void Ship::display(GLdouble blockDistance, GLdouble blockAngle, int blockType)
{
  printHighscore();

  glMatrixMode(GL_MODELVIEW);

  if ( warping_ == false && intro_ == false)
    {
      /* First: Change internal variables, do not rotate or translate yet */
      if (blockType == 4)
	{
	  warping_ = true;
	}
      else if (blockType == 3)
	{
	  jumpLength_ = jumpLengthOriginal_ + (jumpLengthOriginal_*0.2);
	}
      else
	jumpLength_ = jumpLengthOriginal_;

      // Adjust ship X angle to block angle
      if (shipAngleX_ > blockAngle)
	shipAngleX_ -= 0.5;
      else if (shipAngleX_ < blockAngle)
	shipAngleX_ += 0.5;
 
      // "Gravity"
      gravity(blockDistance, blockAngle);

      // Jump
      if(jumping_)
	jump();

      // Turn, wiggle or hover
      if(turning_)
	turn();
      else if (wiggling_)
	wiggle();
      else
	hover();

      glPushMatrix(); // Save matrix
      glPushAttrib(GL_CURRENT_BIT); // Save color

      /* Translate */
      glTranslatef(location_[0]+wiggleX_,
		   location_[1]+hoverY_,
		   location_[2]);

      /* ALL rotations */
      glRotatef((shipAngleX_+jumpAngleX_), 10.0, 0.0, 0.0);
      glRotatef(turnAngle_, 0.0, 0.0, -10.0);
      glRotatef((turnAngle_ + wiggleAngle_), 0.0, -10.0, 0.0);

      /* Draw ship */
      drawShip();

      glPopMatrix(); // Restore matrix
      glPopAttrib(); // Restore color

      /* Point camera on ship */
      camera_->LookAtThis(location_[0],location_[1],location_[2]);

      /* Change variables so next time we move forward (and accelerate) */
      if (moving_)
	{  
	  if (currentSpeed_ < velocity_
	      && !jumping_)
	    {
	      currentSpeed_ = currentSpeed_ + 0.001;
	    }

	  // Move forward & up
	  location_[1] = location_[1] + (currentSpeed_ * tan(blockAngle * (M_PI/180)));
	  location_[2] = location_[2] - currentSpeed_; // Speed is constant

	  // Adjust fumes & increase highscore
	  if (shipFumes_ < 0.5)
	    shipFumes_ += 0.01;

	  highscore_ += 0.01;
	}
    }
  else if (warping_ == true)
    warp();
  else if (intro_ == true)
    intro();

}

void Ship::drawShip()
{
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  drawBody();
  drawEngine(1);
  drawEngine(2);
  drawWindshield();
}

void Ship::moveHere(GLint cubeNr)
{
  // Keep ship from falling down on the sides
  if (fabs(cubeNr_+cubeNr) < 3)
    {
      cubeNr_ = cubeNr_+cubeNr;
      turning_ = true;

      if (highscore_ > 0)
	highscore_ -= 0.5;
    }
}

void Ship::jumpShip()
{
  // If we are not moving, start ship. Else jump!
  if (moving_ == false)
    {
      moving_ = true;
      location_[0] = location_[1] = location_[2] = 0.00;
      camera_->still = false;
      camera_->slowZ = false;
      return;
    }
  else if (jumping_ == false && falling_ == false)
    {
      jumpOrigin_ = location_[2];
      jumpDestination_ = location_[2] - (jumpLength_ * currentSpeed_);
      jumpDestination_ = (jumpLength_ * currentSpeed_);
      jumping_ = true;
      shipFumes_ = 1.0;

      if (highscore_ > 0)
	highscore_ -= 0.5;
    }
}

GLdouble* Ship::getPosition()
{
  return location_;
}

/**
 ** INTERNAL FUNCTIONS
 */

void Ship::gravity(GLdouble blockDistance, GLdouble blockAngle)
{
  falling_ = false;

  if (blockDistance != 0.0) // Update die variable
    hereWeDie_ = blockDistance - 20.0;

  if (((blockDistance >= 0.7) || (blockDistance == 0))  // Are we falling down?
      && (!jumping_)   
      /*&& (blockAngle == 0)*/)
    {
      location_[1] = location_[1] - 0.10;
      falling_ = true;
      if (jumpAngleX_ > -50)
	jumpAngleX_--;
    }
  else if (blockDistance < 0.5) // Keep ship near block!
    {
	 location_[1] += 0.01;
      if ((jumpAngleX_ < 0) && !jumping_)
	jumpAngleX_ += 2;
    }
  else if ((blockDistance > 0.5)      // Have we landed and need to
	   && (blockDistance < 0.7)   // adjust ships X angle?
	   && (!jumping_)
	   && (jumpAngleX_ < 0))
    {
	jumpAngleX_ += 2;
    }

  if (blockDistance >= -0.15   // Fixes a problem where you suddenly die
      && blockDistance < 0.00) // for no reason. 
    location_[1] += 0.15;
  

  // Are we dead?
  if ( (blockDistance < -0.15))
    {
      camera_->slowZ = true;
      new (this)Ship(camera_);
    }
  else if (location_[1] < hereWeDie_)
    {
      camera_->slowZ = true;
      new (this)Ship(camera_);
    }
}

void Ship::jump()
{
  GLint jumpAngleMax = 30;

  // Calculate the relative distance we have left in our jump
  GLdouble distanceFactor = 
    fabs((location_[2] - jumpOrigin_) / jumpDestination_);
  GLdouble jumpHeight;

  if (distanceFactor <= 0.50) // We have a long way to go in our jump
    {
      jumpHeight = (0.10 - (currentSpeed_ * sin(distanceFactor * M_PI)))/2;
      location_[1] = location_[1] + jumpHeight;

      if (jumpAngleX_ < jumpAngleMax)
	jumpAngleX_ += 3;

    }
  else if ((distanceFactor <= 0.99) // We are near our destination
	   && (distanceFactor > 0.50))
    {
      jumpHeight = (currentSpeed_ * (1 - sin(distanceFactor * M_PI)))*2.0;

      location_[1] = location_[1] - jumpHeight;

      if ((jumpAngleX_ > 0)
	  && (distanceFactor > 0.800))	  
	{
	jumpAngleX_ -= 2;

	if (jumpAngleX_ < 0)
	  jumpAngleX_ = 0;

	if (shipFumes_ > 0.5)
	  shipFumes_ -= 0.05;
	}
    }
  else // We have arrived!
    {
      jumping_ = false;
      jumpAngleX_ = 0;
      location_[1] -= 0.08;
    } 
}

void Ship::turn()
{
  // Calculate distance we have left in our turn
  GLfloat distanceLeft = location_[0] - cubeNr_;

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
    { location_[0] = location_[0]-turnSpeed_; }
  else
    { location_[0] = location_[0]+turnSpeed_; }

  // Make floats behave!
  if (distanceLeft < 0.01)
    distanceLeft = 0.000;

  // Have we arrived?
  if (distanceLeft == 0)
    {
      location_[0] = cubeNr_;
      turnAngle_ = 0;
      turning_ = false;

      wiggling_ = true;
      wiggleToRight_ = (!movingLeft);
      wiggleAngle_ = 0;
      wiggleX_ = 0.0;
    }
}

void Ship::turnRotateZ(GLboolean movingLeft, GLdouble distanceLeft)
{
  if ( !movingLeft )
    {
      // We are moving right
      if ( (distanceLeft < 0.5) && (turnAngle_ > 0) )
	{
	  // We are close to our destination
	  turnAngle_--;
	}
      else if (turnAngle_ < 30)
	{
	  // We are far away from our destination
	  turnAngle_++;
	}
    }
  else if (movingLeft)
    {
      // We are moving left
      if ( (distanceLeft < 0.5) && (turnAngle_ < 0) )
	{
	  // We are close to our destination
	  turnAngle_++;
	}
      else if (turnAngle_ > -30)
	{
	  // We are far away from our destination
	  turnAngle_--;
	}
    }
}

void Ship::wiggle()
{
  wiggleCounter_++;

  GLint wiggleLength = 10;
  GLint wiggleBack = wiggleLength*2;

  if (wiggleCounter_ < wiggleLength)
    {
      if (wiggleToRight_)
	{
	  wiggleAngle_++;
	  wiggleX_ = wiggleX_ - 0.001;
	}
      else
	{
	  wiggleAngle_--;
	  wiggleX_ = wiggleX_ + 0.001;
	}
    }
  else if ( (wiggleCounter_ >= wiggleLength )
	    && (wiggleCounter_ < wiggleBack) )
    {
      if (wiggleToRight_)
	{
	  wiggleAngle_--;
	  wiggleX_ = wiggleX_ + 0.001;
	}
      else
	{
	  wiggleAngle_++;
	  wiggleX_ = wiggleX_ - 0.001;
	}
    }
  else
    {
      wiggleAngle_ = 0;
      wiggleX_ = 0.0;
      wiggling_ = false;
      wiggleCounter_ = 0;
    }
 
}

void Ship::hover()
{
  if (hoverCounter_ < 100)
    {
      hoverY_ = hoverY_ + hoverHeight_;
      hoverCounter_++;
    }
  else if ( (hoverCounter_ >= 100) && (hoverCounter_ < 200) )
    {
      hoverY_ = hoverY_ - hoverHeight_;
      hoverCounter_++;
    }
  else
    {
      hoverCounter_ = 0;
      hoverY_ = 0;
    }
}

void Ship::warp()
{
  glPushMatrix(); // Save matrix
  glPushAttrib(GL_CURRENT_BIT); // Save color

  warpCounter_ += 0.1;
  shipFumes_ = 1.0;

  glTranslatef(location_[0]+wiggleX_,
	       location_[1]+hoverY_+warpCounter_,
	       location_[2]-warpCounter_*10);

  drawShip();

  glPopMatrix(); // Restore matrix
  glPopAttrib(); // Restore color

  camera_->LookAtThis(location_[0],
		      location_[1], 
		      (location_[2] - (warpCounter_*0.2)));

  if (warpCounter_ >= 10)
    printLevelComplete();

  if (warpCounter_ >= 30)
    {
      readyForNextLevel = true;
      camera_->reset();
    }
 
}

void Ship::intro()
{
  if(location_[2] > 0)
    {
      if((location_[2] <= 30) && (location_[2] > 20))
	  location_[2] = location_[2] - 0.5;
      else if((location_[2] <= 20) && (location_[2] > 5))
	{
	  camera_->still = false;
	  location_[2] = location_[2] - 0.5;
	}
      else if (location_[2] <= 5)
	{
	  location_[2] = location_[2] - 0.05;
	  shipFumes_ -= 0.02;
	}
      else
	  location_[2] = location_[2] - 0.5;

      hover();

    }
  else
    {
      shipFumes_ = 0.00;
      intro_ = false;
    }

  glPushMatrix(); // Save matrix
  glPushAttrib(GL_CURRENT_BIT); // Save color
  
  glTranslatef(location_[0],
	       location_[1]+hoverY_,
	       location_[2]);
  
  drawShip();
  
  glPopMatrix(); // Restore matrix
  glPopAttrib(); // Restore color
  
  camera_->LookAtThis(location_[0],location_[1],location_[2]);

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
  gluSphere(shipBody_, 0.6, 16, 16);

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
  gluCylinder(shipEngine_,
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

  glColor3f(shipFumes_,0.5*shipFumes_,0);
  glTranslatef(0.0,
	       0.0,
	       0.7
	       );
  gluDisk(shipEngineBack_,
	  0, // inner radius
	  0.27, // outer radius
	  12, // slices
	  12 // rings
	  );
  
  // Draw engine "flames"
  
  if(moving_ == true || intro_ == true)
    {
      // Activate flames if they are disabled
      if(flame1_->active == false || flame2_->active == false)
	{
	  flame1_->activate();
	  flame2_->activate();
	}  
      // Draw flame corresponding to the engine currently being drawn
      if(nr == 1)
	{
	    flame1_->draw(shipFumes_*1000);
	}
      else
	{
	    flame2_->draw(shipFumes_*1000);
	}
    }
  else
    {
      // Disable flames if they are disabled
      if(flame1_->active == true || flame2_->active == true)
	{
	  flame1_->disable();
	  flame2_->disable();
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
  gluDisk(shipEngineBack_,
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

  gluSphere(shipWindshield_, 0.27, 16, 16);

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
  ss << "Highscore: " << (GLint) highscore_;
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

  GLdouble* camLocation = camera_->getLocation();

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
  // glubuild2dmipmaps is bad, handled by cpu
  //gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixelData);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  // Following two lines makes mipmaps be created by gpu
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData); 

  free(pixelData);

  return texNum;
}

void Ship::reset()
{
  GLdouble highscore = highscore_;

  new (this)Ship(camera_);

  location_[0] =  location_[1] = 0.0;
  location_[2] = 70.0;
  camera_->slowZ = true;
  camera_->still = true;
  camera_->reset();

  highscore_ = highscore;
}
