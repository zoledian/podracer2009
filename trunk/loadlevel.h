#ifndef LOADLEVEL_H
#define LOADLEVEL_H
#include "block.h"
#include <vector>

class LoadLevel {
public:
     LoadLevel(std::string);
     ~LoadLevel();
     void loadNewLevel(std::string, int);
     void drawLevel();
     std::vector<Block*> getLevelVector();
     
private:
     GLuint loadTexture(char* name);
     GLint textureNr_;

     std::vector<Block*> blocks_;
     GLint textureId_;

};

#endif
