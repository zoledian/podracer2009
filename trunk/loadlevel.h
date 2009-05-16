#ifndef LOADLEVEL_H
#define LOADLEVEL_H
#include "block.h"
#include <vector>

class LoadLevel {
public:
     LoadLevel(std::string);
     ~LoadLevel();
     void loadNewLevel(std::string);
     void drawLevel();
     std::vector<Block*> getLevelVector();
     
private:
     std::vector<Block*> blocks_;

};

#endif
