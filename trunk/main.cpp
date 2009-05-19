#include <iostream>
#include <math.h>
#include "stdlib.h" // needed for 'exit()'

#include "camera.h"
#include "ship.h"
#include "loadlevel.h"
#include "skybox.h"
#include "collision.h"
using namespace std;

#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>

#ifndef GL_VERSION_2_0
#error This program needs OpenGL 2.0 libraries to compile
#endif


/*** 
 *** GLOBAL VARIABLES & OBJECTS 
 **/
Camera* Cam;
Ship* Spaceship;
LoadLevel* loadLevel;
SkyBox* skyBox;
Collision* collision;

/*** 
 *** INPUT FUNCTIONS
 **/
void inputSpecKey(int key, int mouseX, int mouseY)
{
  // if(key == GLUT_KEY_F1) /* do something */
  if(key == GLUT_KEY_LEFT)
    Spaceship->moveHere(-1);
  if(key == GLUT_KEY_RIGHT)
    Spaceship->moveHere(1);
}

void inputNormKey(unsigned char key, int mouseX, int mouseY)
{
  // Exit program if 'q' or ESCAPE is pressed.
  if (key == 'q' || key == 'Q' || key == '\27')
    {
      exit(0);
    }
  else if (key == '\32' || key == 'm' || key == 'M')
    {
      Cam->slowZ = false;
      Spaceship->jumpShip();
    }
}


/*** 
 *** DISPLAY FUNCTION
 **/
void display()
{
  // This function is called whenever it is time to render
  //  a new frame; due to the idle()-function below, this
  //  function will get called several times per second

  // Clear framebuffer & zbuffer
  glClearColor(0.3, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Enable Z-buffering
  glEnable(GL_DEPTH_TEST);

  // Enable backface culling
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  skyBox->drawSkyBox(Cam->getLocation());

  loadLevel->drawLevel();
  
  // Use collision to find the distance to the block below
  // the ship and the blocks angle
  collision->setLevelVector(loadLevel->getLevelVector());
  collision->setShipxyz(Spaceship->getPosition());
  collision->findBlockBelow();

  Spaceship->drawShip(collision->getyDistance(), 
		      collision->getAngle(),
		      collision->getBlockBelowType());
  
  //Cam->LookAtThis(0.0,0.0,0.0);

  // Swap front- and backbuffers
  glutSwapBuffers();
}


/*** 
 *** TIMER (frame limiter) FUNCTION
 **/
void timer(int i)
{
   glutTimerFunc(10, timer, i);
   glutPostRedisplay();
} 


/*** 
 *** MAIN
 **/
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
  /* if (!glGetString(GL_SHADING_LANGUAGE_VERSION))
    {
      cout << "Error: Your OpenGL driver does not support OpenGL 2.0 shaders\n";
      // fprintf(stderr, "Error: Your OpenGL driver does not support OpenGL 2.0 shaders\n");
      return 0;
      }*/

  // Register our functions with GLUT
  glutDisplayFunc(display);
  glutKeyboardFunc(inputNormKey);
  glutSpecialFunc(inputSpecKey);
  glutTimerFunc(20, timer, 0);


  Cam = new Camera();
  Spaceship = new Ship(Cam);
  loadLevel = new LoadLevel("level1.dat"); // Testlevel
  skyBox = new SkyBox(); // Skybox
  collision = new Collision();
  
  // Enter GLUT's main loop; this function will never return
  glutMainLoop();

  return 0;
}
