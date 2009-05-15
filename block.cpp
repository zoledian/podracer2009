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
     angle_ = 0;
     
     for(int i = 0; i < 3; i++)
     {
	  coord_[i] = 0;
     }
     
}


void Block::setColor(GLfloat red, GLfloat green, GLfloat blue)
{
     red_ = red;
     green_ = green;
     blue_ = blue;
}

void Block::draw()
{

     glPushMatrix();
     glPushAttrib(GL_CURRENT_BIT); // Save color

     if(type_ == 2) // Ordinary block
     {
 	  
	  glColor3f(red_, green_, blue_); 
	  
	  // Translate to the right coordinates and rotate
	  glTranslatef(coord_[0],coord_[1],coord_[2]);
	  glRotatef(angle_,1,0,0);
	  glutSolidCube(1);
	  
     }
     else if(type_ == 3) // Jump block
     {  
	  glColor3f(0.0, 0.2, 0.3); 
	  
	  // Translate to the right coordinates and rotate
	  glTranslatef(coord_[0],coord_[1],coord_[2]);
	  glRotatef(angle_,1,0,0);

	  // Scale the block
	  glTranslatef(0,0.4,0);
	  glScalef(1,0.2,1.0);
	  
	  glutSolidCube(1);	  
     }

     glPopMatrix();
     glPopAttrib(); // Restore color
}

void Block::setType(int type)
{
     type_ = type;
}

int Block::getType()
{
     return type_;
}

void Block::setAngle(int angle)
{
     angle_ = angle;
}

int Block::getAngle()
{
     return angle_;
}

void Block::setCoord(double* coord)
{
     coord_[0] = coord[0];
     coord_[1] = coord[1];
     coord_[2] = coord[2];
}

double* Block::getCoord()
{
     return coord_;
}
