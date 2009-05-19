#include <GL/glut.h>
//#include "zpr.h"
#include "skybox.h"
#include "stdlib.h"
#include "readjpeg.h"
#include <string.h>
using namespace std;

SkyBox::SkyBox()
{
    textureId = loadTexture("./textures/space.jpg");
}

void SkyBox::drawSkyBox(GLdouble* camPos)
{
  int size = 50;

  // Save states
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glDisable(GL_LIGHTING);

	// disable z
	glDisable(GL_DEPTH_TEST);		

        // Enable Gouraud shading
	glShadeModel(GL_SMOOTH);
	

	// Enable texturing
	glEnable(GL_TEXTURE_2D);

	// Replace colors with that of texture's
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	// Specify texture to use
	glBindTexture(GL_TEXTURE_2D, textureId);

	// Front side
	glFrontFace(GL_CW); 

	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	glNormal3f(0,0,-1);
	glTexCoord2f(0, 0);
	glVertex3f(-size + camPos[0], size + camPos[1], -size + camPos[2]);
	glTexCoord2f(1, 0);
	glVertex3f(size + camPos[0], size + camPos[1], -size + camPos[2]);
	glTexCoord2f(1, 1);
	glVertex3f(size + camPos[0], -size + camPos[1], -size + camPos[2]);
	glTexCoord2f(0, 1);
	glVertex3f(-size + camPos[0], -size + camPos[1], -size + camPos[2]);
	glEnd();
  
	// Right side
	glBegin(GL_POLYGON);
	glNormal3f(1,0,0);
	glTexCoord2f(1, 1);
	glVertex3f(size + camPos[0], -size + camPos[1], size + camPos[2]);
	glTexCoord2f(0, 1);
	glVertex3f(size + camPos[0], -size + camPos[1], -size + camPos[2]);
	glTexCoord2f(0, 0);
	glVertex3f(size + camPos[0], size + camPos[1], -size + camPos[2]);
	glTexCoord2f(1, 0);
	glVertex3f(size + camPos[0], size + camPos[1], size + camPos[2]);
	glEnd(); 

	// Back side
	glBegin(GL_POLYGON);
	glNormal3f(0,0,1);
	glTexCoord2f(1, 1);
	glVertex3f(-size + camPos[0], -size + camPos[1], size + camPos[2]);
	glTexCoord2f(0, 1);
	glVertex3f(size + camPos[0], -size + camPos[1], size + camPos[2]);
	glTexCoord2f(0, 0);
	glVertex3f(size + camPos[0], size + camPos[1], size + camPos[2]);
	glTexCoord2f(1, 0);
	glVertex3f(-size + camPos[0], size + camPos[1], size + camPos[2]);
	glEnd();

	// Left side
	glBegin(GL_POLYGON);
	glNormal3f(-1,0,0);
	glTexCoord2f(1, 1);
	glVertex3f(-size + camPos[0], -size + camPos[1], -size + camPos[2]);
	glTexCoord2f(0, 1);
	glVertex3f(-size + camPos[0], -size + camPos[1], size + camPos[2]);
	glTexCoord2f(0, 0);
	glVertex3f(-size + camPos[0], size + camPos[1], size + camPos[2]);
	glTexCoord2f(1, 0);
	glVertex3f(-size + camPos[0], size + camPos[1], -size + camPos[2]);
	glEnd(); 

	// Top side
	glBegin(GL_POLYGON);
	glNormal3f(0,1,0);
	glTexCoord2f(0, 1);
	glVertex3f(-size + camPos[0], size + camPos[1], -size + camPos[2]);
	glTexCoord2f(0, 0);
	glVertex3f(-size + camPos[0], size + camPos[1], size + camPos[2]);
	glTexCoord2f(1, 0);
	glVertex3f(size + camPos[0], size + camPos[1], size + camPos[2]);
	glTexCoord2f(1, 1);
	glVertex3f(size + camPos[0], size + camPos[1], -size + camPos[2]);
	glEnd();

	// Bottom side
	glBegin(GL_POLYGON);
	glNormal3f(0,-1,0);
	glTexCoord2f(1, 0);
	glVertex3f(size + camPos[0], -size + camPos[1], -size + camPos[2]);
	glTexCoord2f(1, 1);
	glVertex3f(size + camPos[0], -size + camPos[1], size + camPos[2]);
	glTexCoord2f(0, 1);
	glVertex3f(-size + camPos[0], -size + camPos[1], size + camPos[2]);
	glTexCoord2f(0, 0);
	glVertex3f(-size + camPos[0], -size + camPos[1], -size + camPos[2]);
	glEnd();

  	/* Restore states */
	glPopClientAttrib();
	glPopMatrix();
	glPopAttrib();
}

GLuint SkyBox::loadTexture(char* name)
{
   GLuint texNum;
  int width = 0, height = 0;
  char* pixelData = 0;
  int nameLen = strlen(name);

  if ((nameLen >= 4 && (!strcmp(name + nameLen - 4, ".jpg")
			|| !strcmp(name + nameLen - 4, ".JPG")))
      || (nameLen >= 5 && (!strcmp(name + nameLen - 5, ".jpeg")
			   || !strcmp(name + nameLen - 5, ".JPEG"))))
    {
      read_JPEG_file(name, &pixelData, &width, &height);
    }
  /* We dont need PPM, less things to worry about

  else if (nameLen >= 4 && (!strcmp(name + nameLen - 4, ".ppm")
			    || !strcmp(name + nameLen - 4, ".PPM")))
    {
      pixelData = readppm(name, &width, &height);
    }
  */

  if (!pixelData)
    exit(0);

  glGenTextures(1, &texNum);
  glBindTexture(GL_TEXTURE_2D, texNum);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0,
	       GL_RGB, GL_UNSIGNED_BYTE, pixelData);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  free(pixelData);

  return texNum;
}
