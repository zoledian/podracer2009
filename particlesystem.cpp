/* Original code from:
   http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=19 */
#include <iostream>
#include <cmath>
#include "particlesystem.h"
#include "particle.h"
#include <GL/glut.h>
#include <stdlib.h>
using namespace std;

ParticleSystem::ParticleSystem(){
  maxParticles = 1000;
  slowdown=2.0f;
  xspeed = 0.0f;
  yspeed = 0.0f;
  col = 1;
  active = false;
  
  // Rainbow Of Colors
  GLfloat colors2[12][3]=
    {
      {1.0f,0.5f,0.5f},{1.0f,0.75f,0.5f},{1.0f,1.0f,0.5f},{0.75f,1.0f,0.5f},
      {0.5f,1.0f,0.5f},{0.5f,1.0f,0.75f},{0.5f,1.0f,1.0f},{0.5f,0.75f,1.0f},
      {0.5f,0.5f,1.0f},{0.75f,0.5f,1.0f},{1.0f,0.5f,1.0f},{1.0f,0.5f,0.75f}
    };

  // Copy colors UGLY hack
  for (int i = 0; i < 12; i++)
    {
  
      for(int y = 0; y < 3; y++)
	{
	  colors[i][y] = colors2[i][y];
	}
    }

  // No particles activated by default
  for (int i = 0 ;i < maxParticles ;i++)
    {
      particles[i].active=false;
      particles[i].life=-1.0f;
    }
}

// Activate all particles
void ParticleSystem::activate()
{
  active = true;
  for (int i = 0 ;i < maxParticles ;i++)
    {
      particles[i].active=true;
      particles[i].life=-1.0f;
    }

}

// Disable all particles
void ParticleSystem::disable()
{
  active = false;
  for (int i = 0 ;i < maxParticles ;i++)
    {
      particles[i].active=false;
      particles[i].life=-1.0f;
    }

}

void ParticleSystem::draw(int nr)
{
  GLdouble colorAdjust;
  if (nr < 1000)
    colorAdjust = 0.2;
  else
    colorAdjust = 0;

  glPushMatrix(); // Save matrix
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

  // Make the fire begin behind the engine
  glTranslatef(0,0,0.05);
	
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST); // Disables Depth Testing
  glClearColor(0.0f,0.0f,0.0f,0.0f); // Black Background
  glClearDepth(1.0f); // Depth Buffer Setup
  glEnable(GL_BLEND); // Enable Blending
  glBlendFunc(GL_SRC_ALPHA,GL_ONE); // Type Of Blending To Perform
  glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST); // Really Nice Perspective Calculations
  glHint(GL_POINT_SMOOTH_HINT,GL_NICEST); // Really Nice Point Smoothing
    
  // Loop through all particles
  for (int i = 0; i < nr; i++)			       
    {
      if (particles[i].active)
	{
	  float x=particles[i].x;
	  float y=particles[i].y;
	  float z=particles[i].z;
	  	  
	  // Draw The Particle Using Our RGB Values, Fade The Particle Based On It's Life
	  glColor4f(particles[i].r,
		    particles[i].g,
		    particles[i].b,
		    particles[i].life - colorAdjust);
	  
	  glBegin(GL_TRIANGLE_STRIP); // Build Quad From A Triangle Strip
	  glVertex3f(x+0.01f,y+0.01f,z); // Top Right
	  glVertex3f(x-0.01f,y+0.01f,z); // Top Left
	  glVertex3f(x+0.01f,y-0.01f,z); // Bottom Right
	  glVertex3f(x-0.01f,y-0.01f,z); // Bottom Left
	  glEnd();

	  glBegin(GL_TRIANGLE_STRIP); // Build Quad Lying Down From A Triangle Strip
	  glVertex3f(x+0.01f,y,z-0.01f);
	  glVertex3f(x-0.01f,y,z+0.01f);
	  glVertex3f(x+0.01f,y,z+0.01f);
	  glVertex3f(x-0.01f,y,z-0.01f);
	  glEnd();
	  
	  particles[i].x+=particles[i].xi/(slowdown*1000); // Move On The X Axis By X Speed
	  particles[i].y+=particles[i].yi/(slowdown*1000); // Move On The Y Axis By Y Speed
	  particles[i].z+=particles[i].zi/(slowdown*1000); // Move On The Z Axis By Z Speed
	  
	  particles[i].life-=particles[i].fade; // Reduce Particles Life By 'Fade'
	  
	  if (particles[i].life<0.0f) // If Particle Is Burned Out
	    {
	      particles[i].life=1.0f; // Give It New Life
	      particles[i].fade=float(rand()%100)/1000.0f+0.003f;

	      particles[i].x = float(((rand()%54)-27.0f)/100);
	      particles[i].y = float(((rand()%54)-27.0f)/100);
	      particles[i].z=0.0f;

	      // Create particles in a circle with radius 0.27
	      while(sqrt((particles[i].x*particles[i].x) + (particles[i].y*particles[i].y)) > 0.27)
		{
		  particles[i].x = float(((rand()%54)-27.0f)/100);
		  particles[i].y = float(((rand()%54)-27.0f)/100);
		}
	      
	      particles[i].xi = float((rand()%10)-5.0f); // Random Speed On X Axis
	      particles[i].yi = float((rand()%10)-5.0f); // Random Speed On Y Axis
	      particles[i].zi = float((rand()%10))*10.0f; // Random Speed On Z Axis
	      
	      // No negative speed on z-axis
	      while(particles[i].zi < 10)
		particles[i].zi=float((rand()%10))*10.0f;

	      particles[i].r=colors[col][0] - colorAdjust; // Select Red From Color Table
	      particles[i].g=colors[col][1] - colorAdjust; // Select Green From Color Table
	      particles[i].b=colors[col][2] - colorAdjust; // Select Blue From Color Table
	    }
	}
      
    }

  glPopMatrix(); // Restore matrix
  glPopAttrib(); // Restore color
  glPopClientAttrib();

}
