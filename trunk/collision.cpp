#include <iostream>
#include <GL/glut.h>
#include <vector>
#include <cmath>
#include "block.h"
#include "collision.h"
using namespace std;

Collision::Collision()
{
     angle_ = 0;
     yDistance_ = 0;

     for(int i = 0; i < 3 ; i++)
     {
	  xyz_[i] = 0;
     }
}

// Set the coordinates the ship has at the moment
void Collision::setShipxyz(GLdouble* xyz)
{
     for(int i = 0; i < 3; i++)
     {
	  xyz_[i] = xyz[i];
     }
}

// Set the vector to use in search for collisions etc
void Collision::setLevelVector(vector<Block*> level)
{
     level_ = level;
}

void Collision::findBlockBelow()
{
     int searchA = 0;
     int searchB = level_.size();

     /* SPEED OPTIMIZED, BUT WITH BUGS!!! */
     /*    // Find the closest block that is below xyz_
     while((searchB - searchA) > 1)
     {
	  cout << "searchA: " << searchA << endl;
	  cout << "searchB: " << searchB << endl;

	  // Search in the half the wanted block is in
	  if(xyz_[2] > ((level_[(searchA + searchB) / 2]->getCoord()[2]))) // Find Z
	  {
	       searchB = ((searchA + searchB) / 2);
	  }
	  else if(xyz_[2] < ((level_[(searchA + searchB) / 2]->getCoord()[2])))
	  {
	       searchA = ((searchA + searchB) / 2);
	  }
	  else if(xyz_[0] < ((level_[(searchA + searchB) / 2]->getCoord()[0]))) // Find X
	  {
	       searchB = ((searchA + searchB) / 2);
	  }
	  else if(xyz_[0] > ((level_[(searchA + searchB) / 2]->getCoord()[0])))
	  {
	       searchA = ((searchA + searchB) / 2);
	  }
	  else
	  {
	       searchA = (searchA + searchB) / 2;
	       searchB = (searchA + searchB) / 2;		    
	       }
	       }*/

/* USED WITH OPTIMIZED SEARCH, BUGGY */
/*    // Check if we've found a block and return
     if( abs(level_[searchA]->getCoord()[2] - xyz_[2]) <= 0.5 && 
	 abs(level_[searchA]->getCoord()[0] - xyz_[0]) <= 0.5 )
     {
	  angle_ = level_[searchA]->getAngle();
	  yDistance_ = ( xyz_[1] - level_[searchA]->getCoord()[1]  );
     }
     else if( abs(level_[searchB]->getCoord()[2] - xyz_[2]) <= 0.5 && 
	      abs(level_[searchB]->getCoord()[0] - xyz_[0]) <= 0.5 )
     {
	  angle_ = level_[searchB]->getAngle();
	  yDistance_ = ( xyz_[1] - level_[searchB]->getCoord()[1]  );
     }
     else
     {
	  angle_ = 0;
	  yDistance_ = 0;
	  }*/

     bool found = false;
     while(searchA <= (searchB - 1) && found == false)
     {
	  if( abs(level_[searchA]->getCoord()[2] - xyz_[2]) >= 0.7 ||
	      abs(level_[searchA]->getCoord()[0] - xyz_[0]) >= 0.7 )
	  {
	       searchA++;
	  }
	  else   
	  {
	       found = true;
	  }
     }
     

     if(found == true)
     {
	  angle_ = level_[searchA]->getAngle();
	  yDistance_ = xyz_[1] - level_[searchA]->getCoord()[1];
     }
     else
     {
	  angle_ = 0;
	  yDistance_ = 0;
     }
 
}

GLdouble Collision::getyDistance()
{
     return yDistance_;
}

GLdouble Collision::getAngle()
{
     return angle_;
}
