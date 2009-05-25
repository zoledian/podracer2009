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

     init();
}

void Block::init()
{
  // Create a display list for a cube
  cubeList_ = glGenLists(1);
  glNewList(cubeList_,GL_COMPILE); 
  glPushMatrix();
  {
    glBegin(GL_QUAD_STRIP);
    // Sides
    glNormal3f(1,0,0);
    glTexCoord2f(0.0, 1.0); glVertex3f( 0.5, 0.5, 0.5);
    glTexCoord2f(0.0, 0.0); glVertex3f( 0.5,-0.5, 0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f( 0.5, 0.5,-0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f( 0.5,-0.5,-0.5);
    glNormal3f(0,0,-1);
    glTexCoord2f(2.0, 1.0); glVertex3f(-0.5, 0.5,-0.5);
    glTexCoord2f(2.0, 0.0); glVertex3f(-0.5,-0.5,-0.5);
    glNormal3f(-1,0,0);
    glTexCoord2f(3.0, 1.0); glVertex3f(-0.5, 0.5, 0.5);
    glTexCoord2f(3.0, 0.0); glVertex3f(-0.5,-0.5, 0.5);
    glNormal3f(0,0,1);
    glTexCoord2f(4.0, 1.0); glVertex3f( 0.5, 0.5, 0.5);
    glTexCoord2f(4.0, 0.0); glVertex3f( 0.5,-0.5, 0.5);
    glEnd();

    glBegin(GL_QUADS);
    // Top
    glNormal3f(0,1,0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, 0.5,-0.5);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, 0.5, 0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f( 0.5, 0.5, 0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f( 0.5, 0.5,-0.5);
    // Bottom
    glNormal3f(0,-1,0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.5,-0.5, 0.5);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.5,-0.5,-0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f( 0.5,-0.5,-0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f( 0.5,-0.5, 0.5);
    glEnd();
  }
  glPopMatrix();
  glEndList(); 

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

	  glPolygonMode(GL_FRONT,GL_FILL);

	  // Translate to the right coordinates and rotate
	  glTranslatef(coord_[0],coord_[1],coord_[2]);
	  glRotatef(angle_,1,0,0);
	  glTranslatef(0,0.4,0);
	  glScalef(1,0.2,1.0);

	  glCallList(cubeList_);
 

     }
     else if(type_ == 3) // Jump block
       { 
	 glDisable(GL_LIGHTING);

	 glColor3f(0.0, 0.2, 0.3); 
	  
	  // Translate to the right coordinates and rotate
	  glTranslatef(coord_[0],coord_[1],coord_[2]);
	  glRotatef(angle_,1,0,0);

	  // Scale the block
	  glTranslatef(0,0.4,0);
	  glScalef(1,0.2,1.0);
	  
	  glCallList(cubeList_);

	  glEnable(GL_LIGHTING); 
     }
     if(type_ == 4) // Ordinary block (finish)
     {
 	  
	  glColor3f(red_, green_, blue_); 
	  
	  // Translate to the right coordinates and rotate
	  glTranslatef(coord_[0],coord_[1],coord_[2]);
	  glRotatef(angle_,1,0,0);
	  glCallList(cubeList_);

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
