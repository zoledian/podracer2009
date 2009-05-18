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
     blockBelowType_ = 1;
     blockBelowIndex_ = -1;
     blockAngle = 0;
     blockZbegin = 0;
     blockZend = 0;
     diffY = 0;
     diffZ = 0;
     traveledDistance = 0;

     for(int i = 0; i < 2; i++)
       {
	 beginCoord[i] = 0;
	 endCoord[i] = 0;
       }

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
     //int searchA = 0;
     //int searchB = level_.size();

     int searchA = level_.size() -1;
     int searchB = 0;

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

     // Since blockBelowIndex_ is used to index a vector
     if(blockBelowIndex_ == -1)
       {
	 blockBelowIndex_ = 0;
       }

     // Check if the ship is still above the same block
     if( (xyz_[2] <= blockZbegin) && (xyz_[2] >= blockZend) )
     {
	  if( ((level_[blockBelowIndex_]->getCoord()[0] - xyz_[0]) <= 0.5) && 
	      ((level_[blockBelowIndex_]->getCoord()[0] - xyz_[0]) >= -0.5) )
	  {
	       found = true;
	  }
	  else
	  {
	       found = false;
	  }
     }


     while(searchA >= searchB && found == false)
     {
	  blockAngle = level_[searchA]->getAngle();
	  
	  // Z begin and end coordinates on the top of the block
	  blockZbegin = level_[searchA]->getCoord()[2] + ((sqrt(2)/2) * cos((45-blockAngle)*(M_PI/180)));
	  blockZend = blockZbegin - cos(blockAngle*(M_PI/180));
	  
	  
	  // Check if the ship is above the block
	  if( (xyz_[2] <= blockZbegin) && (xyz_[2] >= blockZend) )
	  {
	       if( ((level_[searchA]->getCoord()[0] - xyz_[0]) <= 0.5) && 
		   ((level_[searchA]->getCoord()[0] - xyz_[0]) >= -0.5) )
	       {
		    found = true;
		    blockBelowIndex_ = searchA;
		    angle_ = level_[searchA]->getAngle();
		    blockBelowType_ = level_[searchA]->getType();
	       }
	       else
	       {
		    searchA--;
	       }
	  }
	  else
	  {
	       searchA--;
	  }
     }	 
     
     // Reset data if we didn't find a block
     if(found == false)
     {
	  angle_ = 0;
	  yDistance_ = 0;
	  blockBelowType_ = 1;
	  blockBelowIndex_ = -1;
     }

     
}   

GLdouble Collision::getyDistance()
{
  setYdistance();
  return yDistance_;
}

GLdouble Collision::getAngle()
{
     return angle_;
}

int Collision::getBlockBelowType()
{
  return blockBelowType_;
}

void Collision::setYdistance()
{
  if(blockBelowIndex_ != -1)
    { 
      // Begin coordinates on the top of the block
      beginCoord[0] = level_[blockBelowIndex_]->getCoord()[1] - ((sqrt(2)/2) * sin((angle_-45)*(M_PI/180)));
      beginCoord[1] = level_[blockBelowIndex_]->getCoord()[2] + ((sqrt(2)/2) * cos((angle_-45)*(M_PI/180)));

      // End coordinates on the top of the block
      endCoord[0] = beginCoord[0] + sin(angle_*(M_PI/180));
      endCoord[1] = beginCoord[1] - cos(angle_*(M_PI/180));
	
      // How much does Y,Z change when you fly over the block
      diffY = endCoord[0] - beginCoord[0];
      diffZ = beginCoord[1] - endCoord[1];
    
      // Calculate how many percent of the block the ship has traveled
      traveledDistance = (-xyz_[2] + beginCoord[1]) / diffZ;
      
      // Calc. the distance from block to ship
      yDistance_ = xyz_[1] - (beginCoord[0] + (diffY * traveledDistance)); 
    }
  else
    {
      yDistance_ = 0;
    }
}
