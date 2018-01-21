#include "World.hpp"

/*
                      POLYGON
*/

void Polygon::setVertices(std::vector<std::array<double,2> >& vertices) {
  std::vector<std::array<double,2> >::iterator i;
  for(i=vertices.begin();i!=vertices.end();++i) {
    m_vertices.push_back(*i);
  }
}

std::vector<std::array<double,2> > getVertices() {
  return m_vertices;
}

void Polygon::findIntersection(const std::array<double,3>& eqn1,
  const std::array<double,3>& eqn2, std::array<double,2> intersection) {
  if ((eqn1[0]==eqn2[0] && eqn1[1]==eqn2[1]) ||
      (eqn1[0]==0 && eqn2[0]==0) ||
      (eqn1[1]==0 && eqn2[1]==0)) {       // parallel lines
    return;
  }
  if (eqn1[1]==0) {
    double x = -eqn1[2]/eqn1[0];
    double y = -eqn2[0]*x/eqn2[1] + eqn2[2]/eqn2[1];
    intersection[0] = x;
    intersection[1] = y;
    return;
  }
  if (eqn2[1]==0) {
    double x = -eqn2[2]/eqn2[0];
    double y = -eqn1[0]*x/eqn1[1] + eqn1[2]/eqn1[1];
    intersection[0] = x;
    intersection[1] = y;
    return;
  }
  double x = (eqn2[2]/eqn2[1]-eqn1[2]/eqn1[1])/(eqn2[0]/eqn2[1]-eqn1[0]/eqn1[1]);
  double y = -eqn1[0]*x/eqn1[1] + eqn1[2]/eqn1[1];
  intersection[0] = x;
  intersection[1] = y;
  return;
}

void Polygon::closestIntersection(std::array<double,2>& ray,std::array<double,2>& source,
  std::array<double,2>& intersection) {
    std::array<double,3> ray_equation;
    

    double best_dist = std::numeric_limits<double>::infinity();
    std::array<double,2> best_pt;
    size_t n_sides = m_equations.size();
    for (size_t i=0;i != n_sides;++i) {
      std::array<double,2> point;
      this->findIntersection()
    }
    std::array<double,2> point
}

void Polygon::generateEquations(std::vector<std::array<double,2> >& vertices) {
  size_t num_vert = vertices.size();
  for (size_t i = 0;  != num_vert; ++i) {
    std::array<double,2> pointA = vertices[i];
    std::array<double,2> pointB = (i != num_vert) ? vertices[i+1] : vertices[0];
    double a = pointA[1]-pointB[1];
    double b = pointB[0]-pointA[0];
    double c = pointA[0]*pointB[1]-pointB[0]*pointA[1];
    std::array<double,3> eqn = {a,b,c};
    m_equations.push_back(eqn);
  }
}


/*
                      WORLD
*/
