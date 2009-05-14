#include <iostream>
#include "block.h"
#include <GL/glut.h>
using namespace std;

Block::Block()
{
     red_ = 0.3;
     green_ = 0.3;
     blue_ = 0.3;
     type_ = 0;
     jump_ = false;
     angle_ = 0;
     height_ = 0;
}


void Block::setColor(GLfloat red, GLfloat green, GLfloat blue)
{
     red_ = red;
     green_ = green;
     blue_ = blue;
}

void Block::draw()
{
     if(type_ == 2) // Ordinary block
     {
	  glColor3f(red_, green_, blue_); 
	  glutSolidCube(1);
     }
     else if(type_ == 3) // Jump block
     {  
	  glPushMatrix(); // Save matrix
	  glPushAttrib(GL_CURRENT_BIT); // Save color
	  
	  // Change angle for jump block
	  glTranslatef(0,0.4,0);
	  glTranslatef(0,0.1,0.5);
	  glRotatef(35,1,0,0);
	  glTranslatef(0,-0.1,-0.5);

	  // Scale the block
	  glScalef(1,0.2,1.0);
	  
	  glColor3f(0.0, 0.2, 0.3); 
	  glutSolidCube(1);
	  
	  glPopMatrix(); // Restore matrix
	  glPopAttrib(); // Restore color
     }
}

void Block::setType(int type)
{
     type_ = type;
}

int Block::getType()
{
     return type_;
}

void Block::setJump(bool jump)
{
     jump_ = jump;
}

void Block::setAngle(int angle)
{
     angle_ = angle;
}

int Block::getAngle()
{
     return angle_;
}

void Block::setHeight(int height)
{
     height_ = height;
}

int Block::getHeight()
{
     return height_;
}
