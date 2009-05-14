#include <iostream>
#include "block.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
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
	//int line = 0;
	//int position = 0;
	char read[256];
	int data = 0;
	
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
			// Create a block that should be drawn
			if(data == 1)
			{
				Block* newBlock = new Block();
				newBlock->setType(1);

				// Check if this is a jumpblock
				is>>data;
				if(data == 1)
				{
				     newBlock->setJump(true);
				}
				
				blocks_.push_back(newBlock);
				newBlock = 0;
				delete newBlock;
			}
			// Create a block that shouldn't be drawn
			else if(data == 0)
			{
			     Block* newBlock = new Block();
			     newBlock->setType(0);
			     
                             // The next chunk of data is irrelevant
			     is>>data;

			     blocks_.push_back(newBlock);
			     newBlock = 0;
			     delete newBlock;
			}
			// Create an information block
			else if(data == 2)
			{
			     Block* newBlock = new Block();
			     newBlock->setType(2);

			     // Read the angle that should be used for the row
			     is>>data;
			     newBlock->setAngle(data);

			     // Read the height difference that should be used for the row
			     is>>data;
			     newBlock->setHeight(data);

			     blocks_.push_back(newBlock);
			     newBlock = 0;
			     delete newBlock;
			}
			
		}
		
	}

	glPopMatrix();
	

}

// Draw the blocks in the level
void LoadLevel::drawLevel()
{
     int angle = 0;

     glPushMatrix();
     
     for(unsigned int i = 0; i < blocks_.size(); i++)
     {
	  // Check if this is an information block
	  if(blocks_[i]->getType() == 2)
	  {	       
	       // Special case if we're at the beginning
	       if(i != 0)
	       {
		    // It is a new row, reset the position
		    glTranslatef(-5, 0, -1);

		    // Change hight (Y-axis)
		    glTranslatef(0, blocks_[i]->getHeight(), 0);
		    
		    angle = blocks_[i]->getAngle();
		    
		    // Apply the new angle
		    if(angle != 0)
		    {
			 glTranslatef(0,0.5,0.5);
			 glRotatef(angle,1,0,0);
			 glTranslatef(0,-0.5,-0.5);
			 }
		    
	       }
	       else
	       {
		    // Move to first position and make the Y-axis below the ship
		    glTranslatef(-2, -1, 0);
	       }
	       
	  }
	  else
	  {
	       if(angle != 0)
	       {	
		    blocks_[i]->draw();
	       }
	       else
	       {
		    blocks_[i]->draw(); 
	       }
	       
	       // Move to next position
	       glTranslatef(1,0,0);
	       
	  }
     }
     
     glPopMatrix();
}
