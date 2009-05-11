#include "ship.h"
#include <GL/glut.h>
using namespace std;

Ship::Ship()
{
}

void Ship::moveShip(GLdouble x, GLdouble y, GLdouble z)
{
  drawShip(0.0,0.0,0.0);
}

void Ship::specKey(int key, int mouseX, int mouseY)
{
  // if(key == GLUT_KEY_F1) /* do something */
  // if(key == GLUT_KEY_UP) /* ditto */
}

void Ship::drawShip(GLdouble x, GLdouble y, GLdouble z)
{
  glMatrixMode(GL_MODELVIEW);

  /* Draw Cuboid "body" */
  glPushMatrix();
  glColor3f(1,0,0);
  glScalef(0.7,0.3,1); //Size of cuboid
  glutSolidCube(1);
  glPopMatrix();

  /* Draw Sphere "windshield" */
  glPushMatrix();
  glColor3f(0,1,0);
  glTranslatef(0,0,-0.3);
  glutSolidSphere(0.3, // Radius
		  10,  // slices 
		  10  // stacks
		  );
  glPopMatrix();
}
