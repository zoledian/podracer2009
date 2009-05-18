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
     int getBlockBelowType();

private:
     GLdouble angle_;
     GLdouble yDistance_;
     GLdouble xyz_[3];
     GLdouble blockAngle;
     GLdouble blockZend;
     GLdouble blockZbegin;
     GLdouble beginCoord[2];
     GLdouble endCoord[2];
     GLdouble diffY;
     GLdouble diffZ;
     GLdouble traveledDistance;
     int blockBelowType_;
     int blockBelowIndex_;
     std::vector<Block*> level_;
     
     void setYdistance();
};

#endif
