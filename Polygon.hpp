#ifndef Polygon_hpp
#define Polygon_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>
#include "HelperFunctions.hpp"

struct Polygon
{
  // Vertices must be given in CCW order
  Polygon(std::vector<std::array<double,2> >& vertices) {
    generateEquations(vertices);
    setVertices(vertices);
  }
  void setVertices(std::vector<std::array<double,2> >& vertices);
  std::vector<std::array<double,2> > getVertices();
  void closestIntersection(std::array<double,2>& ray,std::array<double,2>& source,
                            std::array<double,2>& intersection);
  void findIntersection(const std::array<double,3>& eqn1,
                            const std::array<double,3>& eqn2);
private:
  void generateEquations(std::vector<std::array<double,2> >& vertices);
  bool isWithinPoints(const std::array<double,2>& lim1,const std::array<double,2>&lim2,
    const std::array<double,2>& point);
  std::vector<std::array<double,2> > m_vertices;
  std::vector<std::array<double,3> > m_equations;
};



#endif
