#include "glstates.h"
#include <GL/glut.h>
using namespace std;

GLStates::GLStates()
{
    // Place one-time initialization code here
  // Enable Z-buffering
  glEnable(GL_DEPTH_TEST);

  // Enable backface culling
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // glPushAttrib(GL_ALL_ATTRIB_BITS);
  // glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
}
