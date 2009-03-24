#ifndef BLOCK_H
#define BLOCK_H

#include <GL/glut.h>

class Block {
 public:
  Block();
  void setColor(GLfloat, GLfloat, GLfloat);

 private: 
  GLfloat[3] colors;
};

#endif

