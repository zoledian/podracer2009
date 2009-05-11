#ifndef GLSTATES_H
#define GLSTATES_H

#include <GL/glut.h>

class GLStates {
 public:
  GLStates();
  void original();
  void pushAttr();
  void popAttr();

};

#endif
