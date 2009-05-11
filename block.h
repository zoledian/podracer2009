#ifndef BLOCK_H
#define BLOCK_H

#include <GL/glut.h>

class Block {
public:
     Block();
     void setRow(int);
     void setPosition(int);
     void setColor(GLfloat, GLfloat, GLfloat);
     void draw();
     
private:
     int row_;
     int position_;
     GLfloat red_;
     GLfloat green_;
     GLfloat blue_;

};

#endif

