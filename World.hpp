#ifndef World_hpp
#define World_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>
#include "HelperFunctions.hpp"
#include "Polygon.hpp"

class World
{
private:
  std::vector<std::array<double,2> > m_bounds;
  std::vector<Polygon> m_obstacles;

  void initialize();
  void delete_obstacles();
  void add_obstacle(const std::vector<std::array<double,2> >& vertices);
public:
  World() {
      initialize();
  }
  ~World() {
      delete_obstacles();
  }
  void setBounds(const std::vector<std::array<double,2> >& bounds);
  std::vector<std::array<double,2> > getBounds();
  void writeToFile();
};





#endif
