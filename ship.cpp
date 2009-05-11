#include "ship.h"
#include "math.h"
#include <GL/glut.h>
using namespace std;

Ship::Ship()
{
  hereIAm[0] = 0;
  hereIAm[1] = 0;
  hereIAm[2] = 0;

  cubePosition[0] = -2;
  cubePosition[1] = -1;
  cubePosition[2] = 0;
  cubePosition[3] = 1;
  cubePosition[4] = 2;

  cubeFrom = 3; // We start here
  cubeTo = -1;
  
  moving = jumping = false;

  angleZ = 0;
}

void Ship::drawShip()
{
  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();

  if(moving)
    move();

  glTranslatef(hereIAm[0],hereIAm[1],hereIAm[2]);

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

  glPopMatrix();
}

void Ship::moveHere(GLint cubeNR)
{
  cubeTo = cubeFrom+cubeNR;
  moving = true;
}


void Ship::move()
{
  cout << "From: " << hereIAm[0] << " ";
  cout << "(dest " << cubePosition[cubeTo-1] << ") " << endl;

  /* Some calculations */
  float distanceLeft = hereIAm[0] - cubePosition[cubeTo-1];
  if (distanceLeft != 0)
    { distanceLeft = fabs(distanceLeft); }
  
  float distanceTotal = cubePosition[cubeFrom-1] - cubePosition[cubeTo-1];

  bool movingLeft = true;
  if (distanceTotal < 0)
    { movingLeft = false; }

  distanceTotal = fabs(distanceTotal);

  cout << hereIAm[0] << " " << cubePosition[cubeTo-1] << endl;
  cout << "Left " << distanceLeft << " Total " << distanceTotal << endl << endl;

  /* Rotate when moving */
  if ( ((distanceLeft / distanceTotal) < 0.5) && (angleZ < 50))
    {
      angleZ = angleZ + 1;
      glRotatef(angleZ,0,0,1);
    }
  else if ( ((distanceLeft / distanceTotal) >= 0.5) && (angleZ > 0))
    {
      angleZ = angleZ - 1;
      glRotatef(angleZ,0,0,1);
    }

  /* Move by the x axis a bit */
  if(movingLeft)
    { hereIAm[0] = hereIAm[0]-0.01; }
  else
    { hereIAm[0] = hereIAm[0]+0.01; }

  /* Have we arrived? */
  if (hereIAm[0] == cubePosition[cubeTo-1] || distanceLeft == 0)
    {
      cubeFrom = cubeTo;
      cubeTo = -1;
      moving = false;
    }
}
