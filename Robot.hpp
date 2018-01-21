#ifndef Robot_hpp
#define Robot_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include "World.hpp"

class Robot
{
  World m_world;
  double m_step_size;
public:
  Robot();
  writeToFile();
private:
  Move();
  Sense();

};

#endif
