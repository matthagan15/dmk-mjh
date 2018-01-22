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
public:
  World() {
    initialize();
  };
  ~World() {
    delete_obstacles();
  };
  void setBounds(const std::vector<std::array<double,2> >& bounds);
  std::vector<std::array<double,2> > getBounds();
  void writeToFile();
  bool closestIntersection(std::array<double,2>& ray,std::array<double,2>& source,
                            std::array<double,2>& intersection);
  std::vector<Polygon*> m_obstacles;
private:
  std::vector<std::array<double,2> > m_bounds;

  void initialize();
  void delete_obstacles();
  void add_obstacle(const std::vector<std::array<double,2> >& vertices);
};

/////////////////////////////
/////////////////////////////
/////////////////////////////

void World::initialize() {
  std::array<double,2> xy_limits = {{0.0,10.0}};
  std::vector<std::array<double,2> > bounds;
  bounds.push_back(xy_limits);
  bounds.push_back(xy_limits);
  this->setBounds(bounds);

  std::vector<std::array<double,2> > verts = {{{5.0,5.0}},{{6.0,5.0}},{{6.0,6.0}},{{5.0,7.0}}};
  this->add_obstacle(verts);

}

void World::delete_obstacles() {
  size_t num_obs = m_obstacles.size();
  for (size_t i=0; i!=num_obs;++i) {
    delete m_obstacles[i];
  }
  m_obstacles.clear();
}

void World::add_obstacle(const std::vector<std::array<double,2> >& vertices) {
  Polygon* obs = new Polygon(vertices);
  m_obstacles.push_back(obs);
}

void World::setBounds(const std::vector<std::array<double,2> >& bounds) {
  m_bounds = bounds;
  double th = 0.1;  // wall thickness
  double low_x = bounds[0][0];
  double low_y = bounds[1][0];
  double high_x = bounds[0][1];
  double high_y = bounds[1][1];
  std::vector<std::array<double,2> > LEFT = {{{low_x,low_y}},{{low_x+th,low_y}},
                                              {{low_x+th,high_y}},{{low_x,high_y}}};
  std::vector<std::array<double,2> > RIGHT = {{{high_x-th,low_y}},{{high_x,low_y}},
                                              {{high_x,high_y}},{{high_x-th,high_y}}};
  std::vector<std::array<double,2> > TOP = {{{low_x,high_y-th}},{{high_x,high_y-th}},
                                              {{high_x,high_y}},{{low_x,high_y}}};
  std::vector<std::array<double,2> > DOWN = {{{low_x,low_y}},{{high_x,low_y}},
                                              {{high_x,low_y+th}},{{low_x,low_y+th}}};
  this->add_obstacle(LEFT);
  this->add_obstacle(RIGHT);
  this->add_obstacle(TOP);
  this->add_obstacle(DOWN);
}

std::vector<std::array<double,2> > World::getBounds() {
  return m_bounds;
}

void World::writeToFile() {
  std::fstream outfile("World.txt",std::fstream::out);

  outfile << m_bounds[0][1] << "," << m_bounds[1][1] << "\n";

  size_t num_obs = m_obstacles.size();
  for (size_t i=0; i!= num_obs;++i) {
    size_t num_verts = m_obstacles[i]->getVertices().size();
    for(size_t j=0; j!=num_verts;++j) {
      outfile << "(" << m_obstacles[i]->getVertices()[j][0];
      outfile << "," << m_obstacles[i]->getVertices()[j][1] << ")";
    }
    outfile << "\n";
  }
  outfile.close();
}

bool World::closestIntersection(std::array<double,2>& ray,std::array<double,2>& source,
  std::array<double,2>& intersection) {
  size_t num_obs = m_obstacles.size();
  double best_dist = std::numeric_limits<double>::infinity();
  std::array<double,2> best_pt;
  std::array<double,2> point;
  for (size_t i=0; i!= num_obs; ++i) {
    std::cout << "obstacle: " << i << "\n";
    bool found = m_obstacles[i]->closestIntersection(ray,source,point);
    if (found) {
      double dist = euclideanDistance(source,point);
      if (dist < best_dist) {
        best_dist = dist;
        best_pt[0] = point[0];
        best_pt[1] = point[1];
      }
    }
  }
  if (best_dist < std::numeric_limits<double>::infinity()) {
    intersection[0] = best_pt[0];
    intersection[1] = best_pt[1];
    return true;
  }
  return false;
}

#endif
