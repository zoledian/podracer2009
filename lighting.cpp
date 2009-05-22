#include "lighting.h"
#include <GL/glut.h>
using namespace std;

Lighting::Lighting()
{
}

void Lighting::setStandard()
{
  // Position light
  GLfloat light_position[] = { 0.0, 5.0, 5.0, 0.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,  1.0);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.00);
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.00);

  // Set material properties
  GLfloat mat_shininess[] = { 20.0 };
  GLfloat mat_ambient[] = { 0.5, 0.5, 0.5, 0.0 };
  GLfloat mat_diffuseColor[] = { 0.5, 0.5, 0.5, 0.0 };
  GLfloat mat_specularColor[] = { 0.5, 0.5, 0.5, 0.0 };

  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseColor);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specularColor);
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
}

