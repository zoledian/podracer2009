#include <iostream>
#include "block.h"
#include <GL/glut.h>
using namespace std;

void Block::setRow(int row)
{
     row_ = row;
}

void Block::setRowPosition(int rowPosition)
{
     rowPosition_ = rowPosition;
}

void Block::setColor(GLfloat, GLfloat, GLfloat)
{

}

void Block::draw()
{
     // Draw block
  glBegin(GL_POLYGON);
  glColor3f(1, 1, 1);
  glVertex3f(-0.1, 0.1, 0.2);
  glVertex3f(0.1, 0.1, 0.2);
  glVertex3f(0.1, -0.1, 0.2);
  glVertex3f(-0.1, -0.1, 0.2);
  glEnd();
  
  glBegin(GL_POLYGON);
  glColor3f(1, 1, 0);
  glVertex3f(0.1, -0.1, 0.4);
  glVertex3f(0.1, -0.1, 0.2);
  glVertex3f(0.1, 0.1, 0.2);
  glVertex3f(0.1, 0.1, 0.4);
  glEnd(); 

  glBegin(GL_POLYGON);
  glColor3f(0, 1, 1);
  glVertex3f(-0.1, -0.1, 0.4);
  glVertex3f(0.1, -0.1, 0.4);
  glVertex3f(0.1, 0.1, 0.4);
  glVertex3f(-0.1, 0.1, 0.4);
  glEnd();

  glBegin(GL_POLYGON);
  glColor3f(0, 1, 0);
  glVertex3f(-0.1, -0.1, 0.2);
  glVertex3f(-0.1, -0.1, 0.4);
  glVertex3f(-0.1, 0.1, 0.4);
  glVertex3f(-0.1, 0.1, 0.2);
  glEnd(); 

  glBegin(GL_POLYGON);
  glColor3f(1, 0, 0);
  glVertex3f(-0.1, 0.1, 0.2);
  glVertex3f(-0.1, 0.1, 0.4);
  glVertex3f(0.1, 0.1, 0.4);
  glVertex3f(0.1, 0.1, 0.2);
  glEnd();

  glBegin(GL_POLYGON);
  glColor3f(0, 0, 1);
  glVertex3f(0.1, -0.1, 0.2);
  glVertex3f(0.1, -0.1, 0.4);
  glVertex3f(-0.1, -0.1, 0.4);
  glVertex3f(-0.1, -0.1, 0.2);
  glEnd();
}
