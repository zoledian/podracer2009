#ifndef BLOCK_H
#define BLOCK_H

#include <GL/glut.h>

class Block {
public:
     Block();
     void draw();
     void setColor(GLfloat, GLfloat, GLfloat);
     void setType(int);
     void setAngle(int);
     void setCoord(double*);
     int getType();
     int getAngle();
     double* getCoord();

private:
     GLfloat red_;
     GLfloat green_;
     GLfloat blue_;
     int type_;
     int angle_;
     double coord_[3];
};

#endif

