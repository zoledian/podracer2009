#include <iostream>
#include "block.h"
#include <GL/glut.h>
using namespace std;

Block::Block()
{
     row_ = 0;
     position_ = 0;
     red_ = 0.3;
     green_ = 0.3;
     blue_ = 0.3;
}

void Block::setRow(int row)
{
     row_ = row;
}

void Block::setPosition(int position)
{
     // The first position is -2 because 0 is the position in the middle
     position_ = position-2;
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
     glColor3f(red_, green_, blue_); 
     glTranslatef(position_, -1, row_);
     glutSolidCube(1);
     glPopMatrix();
}
