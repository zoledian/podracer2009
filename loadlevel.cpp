#include <iostream>
#include "block.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include "loadlevel.h"
using namespace std;

LoadLevel::LoadLevel(string name)
{
	loadNewLevel(name);
}

LoadLevel::~LoadLevel()
{
	// Delete each block to prevent memory leak
	for(unsigned int i = 0; i < blocks_.size(); i++)
	{
		delete blocks_[i];
	}
}

// Load a new level from file
void LoadLevel::loadNewLevel(string name)
{

	// Delete the level currently loaded and change size of vector to 0
	for(unsigned int i = 0; i < blocks_.size(); i++)
	{
		delete blocks_[i];
	}
	blocks_.resize(0,0);

	const char* fileName = name.c_str();
	string readLine = "";
	char read[256];
	int data = 0;
	double xyzRotatePos[3];
	double xyzRotateNeg[3];
	double xyzMiddle[3];
	int angle = 0;
	int oldAngle = 0;
	int height = 0;

	// Coordinates the next block should rotate around.
	// Middle of block +0.5y -0.5z if angle = 0;
	xyzRotatePos[0] = -2;
	xyzRotatePos[1] = -1.5;
	xyzRotatePos[2] = 0.5;
	xyzRotateNeg[0] = -2;
	xyzRotateNeg[1] = -0.5;
	xyzRotateNeg[2] = 0.5;
	xyzMiddle[0] = -2;
	xyzMiddle[1] = -1;
	xyzMiddle[2] = 0;
	
	ifstream fin(fileName);
	
	glPushMatrix();
	
	// Read one line at a time
	while( fin.getline(read, 256) ) {
	  readLine = read;
	  
	  istringstream is;
	  is.str(readLine);
	  // Check the read data
	  while(is>>data)
	    {					        
	      // Read an information row
	      if(data == 0)
		{
		  oldAngle = angle;
		  // Read the angle that should be used for the next row
		  is>>angle;
	  
		  // Read the height difference that should be used for the next row
		  is>>height; 
		  xyzRotatePos[1] = xyzRotatePos[1] + height;
		  xyzRotateNeg[1] = xyzRotateNeg[1] + height;

		  if((angle - oldAngle) > 0)
		    {		
		      // Calculate the middle of the current block
		      xyzMiddle[1] = xyzRotatePos[1] + ((sqrt(2)/2) * sin((angle+45)*(M_PI/180)));
		      xyzMiddle[2] = xyzRotatePos[2] - ((sqrt(2)/2) * cos((angle+45)*(M_PI/180)));	  

		      // Calculate new x,y for neg x,y since it was rotated away
		      xyzRotateNeg[1] = xyzRotatePos[1] + sin((180-angle-90)*(M_PI/180));
		      xyzRotateNeg[2] = xyzRotatePos[2] + cos((180-angle-90)*(M_PI/180));
		    }
		  else if((angle - oldAngle) < 0)
		    {
		      // Calculate the middle of the current block
		      xyzMiddle[1] = xyzRotateNeg[1] + ((sqrt(2)/2) * sin((angle-45)*(M_PI/180)));
		      xyzMiddle[2] = xyzRotateNeg[2] - ((sqrt(2)/2) * cos((angle-45)*(M_PI/180)));

		      // Calculate new x,y for neg x,y since it was rotated away
		      xyzRotatePos[1] = xyzRotateNeg[1] - sin((180+angle-90)*(M_PI/180));
		      xyzRotatePos[2] = xyzRotateNeg[2] + cos((180+angle-90)*(M_PI/180));
		    }
		  else
		    {
		      // Calculate the middle of the current block
		      xyzMiddle[1] = xyzRotatePos[1] + ((sqrt(2)/2) * sin((angle+45)*(M_PI/180)));
		      xyzMiddle[2] = xyzRotatePos[2] - ((sqrt(2)/2) * cos((angle+45)*(M_PI/180)));
		    }
		  
		  // Calculate y,z for the next block to rotate around
		  xyzRotatePos[1] = xyzRotatePos[1] + sin(angle*(M_PI/180));
		  xyzRotatePos[2] = xyzRotatePos[2] - cos(angle*(M_PI/180));
		  xyzRotateNeg[1] = xyzRotateNeg[1] + sin(angle*(M_PI/180));
		  xyzRotateNeg[2] = xyzRotateNeg[2] - cos(angle*(M_PI/180));
		  
		}			
	      // Create a block
	      else if(data == 2)
		{
		  Block* newBlock = new Block();
		  newBlock->setType(2);
		  newBlock->setAngle(angle);
		  newBlock->setCoord(xyzMiddle);
		  
		  blocks_.push_back(newBlock);
		  newBlock = 0;
		  delete newBlock;			     
		}
	      // Create a jump block
	      else if(data == 3)
		{
		  Block* newBlock = new Block();
		  newBlock->setType(3);
		  newBlock->setAngle(angle+35);
		  
		  // Need new coordinates since it changes with the extra rotation
		  double jumpxyz[3];
		  jumpxyz[0] = xyzMiddle[0];
		  jumpxyz[1] = xyzMiddle[1] + (sin(35*(M_PI/180)) / 2);
		  jumpxyz[2] = xyzMiddle[2] + ((1 - cos(35*(M_PI/180))) / 2);
		  
		  newBlock->setCoord(jumpxyz);
		  
		  blocks_.push_back(newBlock);
		  newBlock = 0;
		  delete newBlock;			     
		}
	      
	      xyzRotatePos[0]++;
	      xyzRotateNeg[0]++;
	      xyzMiddle[0]++;
	      
	      
	    }
	  
	  xyzRotatePos[0] = -2;
	  xyzRotateNeg[0] = -2;
	  xyzMiddle[0] = -2;
	  
	}
	
	glPopMatrix();
	
	
}

// Draw the blocks in the level
void LoadLevel::drawLevel()
{
  glPushMatrix();
  glPushAttrib(GL_CURRENT_BIT); // Save color
  
  for(unsigned int i = 0; i < blocks_.size(); i++)
    {
      blocks_[i]->draw();
    }
  
  glPopMatrix();
  glPopAttrib(); // Restore color
}

vector<Block*> LoadLevel::getLevelVector()
{
  return blocks_;
}
