#ifndef BLOCK_H
#define BLOCK_H

#include <GL/glut.h>

class Block {
public:
     void setRow(int);
     void setRowPosition(int);
     void setColor(GLfloat, GLfloat, GLfloat);
     void draw();
     
private:
     int row_;
     int rowPosition_;
     //GLfloat[3] colors;
};

#endif

