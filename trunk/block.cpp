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

	  //glEnable(GL_DEPTH_TEST);
	  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen and z-buffer
	  //glLoadIdentity(); // Reset current matrix (Modelview)
     
	  glPolygonMode(GL_FRONT,GL_FILL);


	  // Translate to the right coordinates and rotate
	  glTranslatef(coord_[0],coord_[1],coord_[2]);
	  glRotatef(angle_,1,0,0);
	  //glutSolidCube(1);
	  glTranslatef(0,0.4,0);
	  glScalef(1,0.2,1.0);
	  

	  // Draw block
	  // Front side
	  glBegin(GL_POLYGON);
	  glNormal3f(0,0,-1);
	  glVertex3f(-0.5, 0.5, -0.5);
	  glVertex3f(0.5, 0.5, -0.5);
	  glVertex3f(0.5, -0.5, -0.5);
	  glVertex3f(-0.5, -0.5, -0.5);
	  glEnd();
	  
	  // Right side
	  glBegin(GL_POLYGON);
	  glNormal3f(1,0,0);
	  glVertex3f(0.5, -0.5, 0.5);
	  glVertex3f(0.5, -0.5, -0.5);
	  glVertex3f(0.5, 0.5, -0.5);
	  glVertex3f(0.5, 0.5, 0.5);
	  glEnd(); 
	  
	  // Back side
	  glBegin(GL_POLYGON);
	  glNormal3f(0,0,1);
	  glVertex3f(-0.5, -0.5, 0.5);
	  glVertex3f(0.5, -0.5, 0.5);
	  glVertex3f(0.5, 0.5, 0.5);
	  glVertex3f(-0.5, 0.5, 0.5);
	  glEnd();
	  
	  // Left side
	  glBegin(GL_POLYGON);
	  glNormal3f(-1,0,0);
	  glVertex3f(-0.5, -0.5, -0.5);
	  glVertex3f(-0.5, -0.5, 0.5);
	  glVertex3f(-0.5, 0.5, 0.5);
	  glVertex3f(-0.5, 0.5, -0.5);
	  glEnd(); 
	  
	  // Top side
	  glBegin(GL_POLYGON);
	  glNormal3f(0,1,0);
	  glVertex3f(-0.5, 0.5, -0.5);
	  glVertex3f(-0.5, 0.5, 0.5);
	  glVertex3f(0.5, 0.5, 0.5);
	  glVertex3f(0.5, 0.5, -0.5);
	  glEnd();
	  
	  // Bottom side
	  glBegin(GL_POLYGON);
	  glNormal3f(0,-1,0);
	  glVertex3f(0.5, -0.5, -0.5);
	  glVertex3f(0.5, -0.5, 0.5);
	  glVertex3f(-0.5, -0.5, 0.5);
	  glVertex3f(-0.5, -0.5, -0.5);
	  glEnd();

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
     if(type_ == 4) // Ordinary block (finish)
     {
 	  
	  glColor3f(red_, green_, blue_); 
	  
	  // Translate to the right coordinates and rotate
	  glTranslatef(coord_[0],coord_[1],coord_[2]);
	  glRotatef(angle_,1,0,0);
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
