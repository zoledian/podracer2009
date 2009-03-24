

/*
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
*/

#include <iostream>
#include <math.h>
using namespace std;

#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>

#ifndef GL_VERSION_2_0
#error This program needs OpenGL 2.0 libraries to compile
#endif

/*
#include "helpers.h"
#include "shaderutils.h"
*/

int init()
{
  // Place one-time initialization code here
  return 1;
}

void display()
{
  // This function is called whenever it is time to render
  //  a new frame; due to the idle()-function below, this
  //  function will get called several times per second

  // Clear framebuffer & zbuffer
  glClearColor(0.3, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Setup projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90, 1, 0.01, 100);

  // Setup object matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  // glMultMatrixd(getObjectMatrix());

  // Enable Z-buffering
  glEnable(GL_DEPTH_TEST);

  // Enable backface culling
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

  // Swap front- and backbuffers
  glutSwapBuffers();
}

void idle()
{
  // This function is called whenever the computer is idle

  // As soon as the machine is idle, ask GLUT to trigger rendering of a new
  // frame
  glutPostRedisplay();
}
int main(int argc, char **argv)
{
  glutInit(&argc, argv);

  // Configure GLUT:
  //  - framebuffer with RGB + Alpha values per pixel
  //  - Z-buffer
  //  - two sets of above mentioned buffers, so that
  //    doublebuffering is possible
  //
  // Initial window size 800x800
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(800, 800);
  glutCreateWindow("Podracer 2009");

  // Ensure that the machine the program is running on has
  //  new enough OpenGL drivers
  if (!glGetString(GL_SHADING_LANGUAGE_VERSION))
    {
      fprintf(stderr, "Error: Your OpenGL driver does not support OpenGL 2.0 shaders\n");
      return 0;
    }

  // Call various init functions
  if (!init())
    return 0;

  // Register our display- and idle-functions with GLUT
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  // glutSpecialFunc(functionKeys);

  // Enter GLUT's main loop; this function will never return
  glutMainLoop();

  return 0;
}
