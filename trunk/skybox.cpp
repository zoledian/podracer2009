#include <GL/glut.h>
#include "skybox.h"
using namespace std;

SkyBox::SkyBox()
{
}

void SkyBox::drawSkyBox()
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
	
/*	
        // Make skybox stick
	GLdouble *cameraMatrix = getCameraMatrix();
	GLdouble newMatrix[16];
	
	int i = 0;
	GLdouble temp;
	while(i<16)
	{
		temp = cameraMatrix[i];
		newMatrix[i] = temp;
		i = i + 1;
	}
	
	newMatrix[12] = 0;
	newMatrix[13] = 0;
	newMatrix[14] = 0;
	
	
	glLoadMatrixd(newMatrix);
*/

	// Draw cube
	//glEnable(GL_TEXTURE_2D);

	// Front side
	glFrontFace(GL_CW); 

	glColor3f(0, 0, 0);
	//glBindTexture(GL_TEXTURE_2D, texturePosZ);
	glBegin(GL_POLYGON);
	glNormal3f(0,0,-1);
	glTexCoord2f(0, 0);
	glVertex3f(-50, 50, -50);
	glTexCoord2f(1, 0);
	glVertex3f(50, 50, -50);
	glTexCoord2f(1, 1);
	glVertex3f(50, -50, -50);
	glTexCoord2f(0, 1);
	glVertex3f(-50, -50, -50);
	glEnd();
  
	// Right side
	//glBindTexture(GL_TEXTURE_2D, texturePosX);
	glBegin(GL_POLYGON);
	glNormal3f(1,0,0);
	glTexCoord2f(1, 1);
	glVertex3f(50, -50, 50);
	glTexCoord2f(0, 1);
	glVertex3f(50, -50, -50);
	glTexCoord2f(0, 0);
	glVertex3f(50, 50, -50);
	glTexCoord2f(1, 0);
	glVertex3f(50, 50, 50);
	glEnd(); 

	// Back side
	//glBindTexture(GL_TEXTURE_2D, textureNegZ);
	glBegin(GL_POLYGON);
	glNormal3f(0,0,1);
	glTexCoord2f(1, 1);
	glVertex3f(-50, -50, 50);
	glTexCoord2f(0, 1);
	glVertex3f(50, -50, 50);
	glTexCoord2f(0, 0);
	glVertex3f(50, 50, 50);
	glTexCoord2f(1, 0);
	glVertex3f(-50, 50, 50);
	glEnd();

	// Left side
	//glBindTexture(GL_TEXTURE_2D, textureNegX);
	glBegin(GL_POLYGON);
	glNormal3f(-1,0,0);
	glTexCoord2f(1, 1);
	glVertex3f(-50, -50, -50);
	glTexCoord2f(0, 1);
	glVertex3f(-50, -50, 50);
	glTexCoord2f(0, 0);
	glVertex3f(-50, 50, 50);
	glTexCoord2f(1, 0);
	glVertex3f(-50, 50, -50);
	glEnd(); 

	// Top side
	//glBindTexture(GL_TEXTURE_2D, texturePosY);
	glBegin(GL_POLYGON);
	glNormal3f(0,1,0);
	glTexCoord2f(0, 1);
	glVertex3f(-50, 50, -50);
	glTexCoord2f(0, 0);
	glVertex3f(-50, 50, 50);
	glTexCoord2f(1, 0);
	glVertex3f(50, 50, 50);
	glTexCoord2f(1, 1);
	glVertex3f(50, 50, -50);
	glEnd();

	// Bottom side
	//glBindTexture(GL_TEXTURE_2D, textureNegY);
	glBegin(GL_POLYGON);
	glNormal3f(0,-1,0);
	glTexCoord2f(1, 0);
	glVertex3f(50, -50, -50);
	glTexCoord2f(1, 1);
	glVertex3f(50, -50, 50);
	glTexCoord2f(0, 1);
	glVertex3f(-50, -50, 50);
	glTexCoord2f(0, 0);
	glVertex3f(-50, -50, -50);
	glEnd();

  	/* Restore states */
	glPopClientAttrib();
	glPopMatrix();
	glPopAttrib();
}
