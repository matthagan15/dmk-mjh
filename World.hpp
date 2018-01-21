#ifndef World_hpp
#define World_hpp

#include <iostream>
#include <stdio.h>
#include <vector>
#include <array>

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
  std::vector<std::array<double,2> > m_vertices;
  std::vector<std::array<double,3> > m_equations;
}


class World
{
private:
  std::vector<std::array<double,2> > m_bounds;
  std::vector<Obstacle> m_obstacles

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
