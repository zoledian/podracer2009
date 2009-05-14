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
     if(type_ == 1)
     {
	  glColor3f(red_, green_, blue_); 
	  glutSolidCube(1);
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
