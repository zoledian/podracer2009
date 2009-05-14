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
     void setHeight(int);
     void setJump(bool);
     int getType();
     int getAngle();
     int getHeight();

private:
     int type_;
     bool jump_;
     GLfloat red_;
     GLfloat green_;
     GLfloat blue_;
     int angle_;
     int height_;
};

#endif

