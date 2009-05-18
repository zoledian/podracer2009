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
     blockZSize = 0;
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

     // Since blockBelowIndex_ is used to index a vector
     if(blockBelowIndex_ == -1)
       {
	 blockBelowIndex_ = 0;
       }


     // Check if the ship is still above the same block
     if( abs(level_[blockBelowIndex_]->getCoord()[2] - xyz_[2]) > blockZSize ||
	 abs(level_[blockBelowIndex_]->getCoord()[0] - xyz_[0]) > 0.5)
       {
     
	 while(searchA <= (searchB - 1) && found == false)
	   {
	     // Calculate the Z size of a block, take angle into account
	     blockAngle = level_[searchA]->getAngle();
	     if(blockAngle > 0)
	       {
		 blockZSize = sin((90 - blockAngle)*(M_PI/180));
	       }
	     else if(blockAngle < 0)
	       {
		 blockZSize = cos(blockAngle*(M_PI/180));
	       }
	     else
	       {
		 blockZSize = 0.5;
	       }
	     
	     // Is the ship above the block?
	     if( abs(level_[searchA]->getCoord()[2] - xyz_[2]) > blockZSize ||
		 abs(level_[searchA]->getCoord()[0] - xyz_[0]) > 0.5 )
	       {
		 searchA++;
	       }
	     else   
	       {
		 found = true;
		 blockBelowIndex_ = searchA;
	       }
	   }
	 

	 // If we found the block, save data
	 if(found == true)
	   {
	     angle_ = level_[searchA]->getAngle();
	     //setYdistance();
	     blockBelowType_ = level_[searchA]->getType();
	   }
	 else
	   {
	     angle_ = 0;
	     yDistance_ = 0;
	     blockBelowType_ = 1;
	     blockBelowIndex_ = -1;
	   }
	 
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
      cout << " CubeY: " << (beginCoord[0] + (diffY * traveledDistance)) << endl;
    }
  else
    {
      yDistance_ = 0;
    }
}
