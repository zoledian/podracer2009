#ifndef COLLISION_H
#define COLLISION_H
#include "block.h"

class Collision {
public:
     Collision();
     void setShipxyz(GLdouble*);
     void setLevelVector(std::vector<Block*> level);
     GLdouble getyDistance();
     GLdouble getAngle();
     void findBlockBelow();

private:
     GLdouble angle_;
     GLdouble yDistance_;
     GLdouble xyz_[3];
     std::vector<Block*> level_;
};

#endif
