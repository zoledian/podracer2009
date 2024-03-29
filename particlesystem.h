/* Original code from:
   http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=19 */
#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "particle.h"
#include <GL/glut.h>

class ParticleSystem{
 public:
  ParticleSystem();
  void activate();
  void disable();
  void draw(int);
  bool active;

 private:
  int maxParticles;
  float	slowdown; // Slow Down Particles
  float	xspeed;
  float	yspeed; 
  GLuint col; // Current Color Selection
  particle particles[1000]; // Particle Array
  GLfloat colors[12][3];
};

#endif
