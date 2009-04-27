

/*
  #include <stdio.h>
  #include <math.h>
  #include <stdlib.h>
*/

#include <iostream>
#include <math.h>
#include "glstates.h"
#include "input.h"
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

void display()
{
  // This function is called whenever it is time to render
  //  a new frame; due to the idle()-function below, this
  //  function will get called several times per second

  // Clear framebuffer & zbuffer
  glClearColor(0.3, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
  // Initiate glut
  glutInit(&argc, argv);

  // Configure GLUT:
  //  - framebuffer with RGB + Alpha values per pixel
  //  - Z-buffer
  //  - two sets of above mentioned buffers, so that
  //    doublebuffering is possible
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

  // Create window
  glutInitWindowSize(800, 800);
  glutCreateWindow("Podracer 2009");

  // Ensure that the machine the program is running on has
  //  new enough OpenGL drivers
  if (!glGetString(GL_SHADING_LANGUAGE_VERSION))
    {
      fprintf(stderr, "Error: Your OpenGL driver does not support OpenGL 2.0 shaders\n");
      return 0;
    }

  // Create our stateHandler, define a default set of states 
  // and push them onto the stack
  GLStates* StateHandler = new GLStates();
  // Input* input = new Input();

  // Register our display- and idle-functions with GLUT
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutKeyboardFunc(Input::normKey);
  glutSpecialFunc(Input::specKey);
  // FIXME: repost, keyboard, functionkeys, mouse...

  // Enter GLUT's main loop; this function will never return
  glutMainLoop();

  return 0;
}
