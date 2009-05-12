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
	int line = 0;
	int position = 0;
	char read[256];
	int data = 0;
	
	ifstream fin(fileName);
	
	// Read one line at a time
	while( fin.getline(read, 256) ) {
		readLine = read;

		istringstream is;
		is.str(readLine);
		// Check the read data
		while(is>>data)
		{
			// Create the corresponding block if we read a 1
			if(data == 1)
			{
				Block* newBlock = new Block();
				newBlock->setPosition(position);
				newBlock->setRow(-line);
				blocks_.push_back(newBlock);
				newBlock = 0;
				delete newBlock;
			}
			
			position++;
		}

		position = 0;
		line++;
	}
	

}

// Draw the blocks in the level
void LoadLevel::drawLevel()
{
	for(unsigned int i = 0; i < blocks_.size(); i++)
	{
		blocks_[i]->draw();
	}
}
