#include "glstates.h"
#include <GL/glut.h>
using namespace std;

GLStates::GLStates()
{
}

void GLStates::original()
{
  // Enable Z-buffering
  glEnable(GL_DEPTH_TEST);

  // Enable backface culling
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

void GLStates::pushAttr()
{
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);  
}

void GLStates::popAttr()
{
  glPopAttrib();
  glPopClientAttrib();
}
