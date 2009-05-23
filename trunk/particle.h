/* Original code from:
   http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=19 */
#ifndef PARTICLE_H
#define PARTICLE_H

typedef struct
{
	bool	active;	// Active (Yes/No)
	float	life; // Particle Life
	float	fade; // Fade Speed
	float	r; // Red Value
	float	g; // Green Value
	float	b; // Blue Value
	float	x; // X Position
	float	y; // Y Position
	float	z; // Z Position
	float	xi; // X Direction
	float	yi; // Y Direction
	float	zi; // Z Direction
	float	xg; // X Gravity
	float	yg; // Y Gravity
	float	zg; // Z Gravity


} particle;

#endif
