#include <GL/glut.h>
#include "zpr.h"
#include "skybox.h"
using namespace std;

SkyBox::SkyBox()
{
}

void SkyBox::drawSkyBox(GLdouble* camPos)
{
	// Save states
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glDisable(GL_LIGHTING);

	// disable z
	glDisable(GL_DEPTH_TEST);		

        // Enable Gouraud shading
	glShadeModel(GL_SMOOTH);
	
	// Draw cube
	//glEnable(GL_TEXTURE_2D);

	// Front side
	glFrontFace(GL_CW); 

	glColor3f(0, 0, 0);
	//glBindTexture(GL_TEXTURE_2D, texturePosZ);
	glBegin(GL_POLYGON);
	glNormal3f(0,0,-1);
	//glTexCoord2f(0, 0);
	glVertex3f(-50 + camPos[0], 50 + camPos[1], -50 + camPos[2]);
	glTexCoord2f(1, 0);
	glVertex3f(50 + camPos[0], 50 + camPos[1], -50 + camPos[2]);
	glTexCoord2f(1, 1);
	glVertex3f(50 + camPos[0], -50 + camPos[1], -50 + camPos[2]);
	glTexCoord2f(0, 1);
	glVertex3f(-50 + camPos[0], -50 + camPos[1], -50 + camPos[2]);
	glEnd();
  
	// Right side
	//glBindTexture(GL_TEXTURE_2D, texturePosX);
	glBegin(GL_POLYGON);
	glNormal3f(1,0,0);
	//glTexCoord2f(1, 1);
	glVertex3f(50 + camPos[0], -50 + camPos[1], 50 + camPos[2]);
	//glTexCoord2f(0, 1);
	glVertex3f(50 + camPos[0], -50 + camPos[1], -50 + camPos[2]);
	//glTexCoord2f(0, 0);
	glVertex3f(50 + camPos[0], 50 + camPos[1], -50 + camPos[2]);
	//glTexCoord2f(1, 0);
	glVertex3f(50 + camPos[0], 50 + camPos[1], 50 + camPos[2]);
	glEnd(); 

	// Back side
	//glBindTexture(GL_TEXTURE_2D, textureNegZ);
	glBegin(GL_POLYGON);
	glNormal3f(0,0,1);
	//glTexCoord2f(1, 1);
	glVertex3f(-50 + camPos[0], -50 + camPos[1], 50 + camPos[2]);
	//glTexCoord2f(0, 1);
	glVertex3f(50 + camPos[0], -50 + camPos[1], 50 + camPos[2]);
	//glTexCoord2f(0, 0);
	glVertex3f(50 + camPos[0], 50 + camPos[1], 50 + camPos[2]);
	//glTexCoord2f(1, 0);
	glVertex3f(-50 + camPos[0], 50 + camPos[1], 50 + camPos[2]);
	glEnd();

	// Left side
	//glBindTexture(GL_TEXTURE_2D, textureNegX);
	glBegin(GL_POLYGON);
	glNormal3f(-1,0,0);
	//glTexCoord2f(1, 1);
	glVertex3f(-50 + camPos[0], -50 + camPos[1], -50 + camPos[2]);
	//glTexCoord2f(0, 1);
	glVertex3f(-50 + camPos[0], -50 + camPos[1], 50 + camPos[2]);
	//glTexCoord2f(0, 0);
	glVertex3f(-50 + camPos[0], 50 + camPos[1], 50 + camPos[2]);
	//glTexCoord2f(1, 0);
	glVertex3f(-50 + camPos[0], 50 + camPos[1], -50 + camPos[2]);
	glEnd(); 

	// Top side
	//glBindTexture(GL_TEXTURE_2D, texturePosY);
	glBegin(GL_POLYGON);
	glNormal3f(0,1,0);
	//glTexCoord2f(0, 1);
	glVertex3f(-50 + camPos[0], 50 + camPos[1], -50 + camPos[2]);
	//glTexCoord2f(0, 0);
	glVertex3f(-50 + camPos[0], 50 + camPos[1], 50 + camPos[2]);
	//glTexCoord2f(1, 0);
	glVertex3f(50 + camPos[0], 50 + camPos[1], 50 + camPos[2]);
	//glTexCoord2f(1, 1);
	glVertex3f(50 + camPos[0], 50 + camPos[1], -50 + camPos[2]);
	glEnd();

	// Bottom side
	//glBindTexture(GL_TEXTURE_2D, textureNegY);
	glBegin(GL_POLYGON);
	glNormal3f(0,-1,0);
	//glTexCoord2f(1, 0);
	glVertex3f(50 + camPos[0], -50 + camPos[1], -50 + camPos[2]);
	//glTexCoord2f(1, 1);
	glVertex3f(50 + camPos[0], -50 + camPos[1], 50 + camPos[2]);
	//glTexCoord2f(0, 1);
	glVertex3f(-50 + camPos[0], -50 + camPos[1], 50 + camPos[2]);
	//glTexCoord2f(0, 0);
	glVertex3f(-50 + camPos[0], -50 + camPos[1], -50 + camPos[2]);
	glEnd();

  	/* Restore states */
	glPopClientAttrib();
	glPopMatrix();
	glPopAttrib();
}
