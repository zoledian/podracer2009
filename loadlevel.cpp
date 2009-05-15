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
	double xyz[3];
	int angle = 0;
	int height = 0;
	xyz[0] = -2;
	xyz[1] = -1;
	xyz[2] = 1;
	
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
			     // Read the angle that should be used for the next row
			     is>>angle;

			     // Read the height difference that should be used for the next row
			     is>>height; 
			     xyz[1] = xyz[1] + height;

			     // Change y,z
			      xyz[1] = xyz[1] + sin(angle*(M_PI/180));
			      xyz[2] = xyz[2] - cos(angle*(M_PI/180));			     
			}
			// Nothing should be done
			else if(data == 1)
			{
			     /* Block* newBlock = new Block();
			     newBlock->setType(1);
			     			     
			     blocks_.push_back(newBlock);
			     newBlock = 0;
			     delete newBlock;  */	
			}			
			// Create a block
			else if(data == 2)
			{
			     Block* newBlock = new Block();
			     newBlock->setType(2);
			     newBlock->setAngle(angle);
			     newBlock->setCoord(xyz);
			     
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
			     jumpxyz[0] = xyz[0];
			     jumpxyz[1] = xyz[1] + (sin(35*(M_PI/180)) / 2);
			     jumpxyz[2] = xyz[2] + ((1 - cos(35*(M_PI/180))) / 2);

			     newBlock->setCoord(jumpxyz);
			     			     
			     blocks_.push_back(newBlock);
			     newBlock = 0;
			     delete newBlock;			     
			}
			
			xyz[0]++;
			
		}
		
		xyz[0] = -2;
		
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
