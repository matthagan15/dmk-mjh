#include "Polygon.hpp"

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

bool Polygon::isWithinPoints(const std::array<double,2>& lim1,const std::array<double,2>&lim2,
  const std::array<double,2>& point) {
  if (lim1[0]==lim2[0]) {
    if (lim1[1] > lim2[1]) {
      return (point[1] <= lim1[1] && point[1] >= lim2[1]);
    } else {:
      return (point[1] >= lim1[1] && point[1] <= lim2[1]);
    }
  } else {
    if (lim1[0] > lim2[0]) {
      return (point[0] <= lim1[0] && point[0] >= lim2[0]);
    } else {:
      return (point[0] >= lim1[0] && point[0] <= lim2[0]);
    }
  }
}

void Polygon::closestIntersection(std::array<double,2>& ray,std::array<double,2>& source,
  std::array<double,2>& intersection) {
  std::array<double,3> ray_eqn;
  double dist_ray = pow(pow(ray[0],2.0)+pow(ray[1],2.0),0.5);
  ray_eqn[0] = -ray[1];
  ray_eqn[1] = ray[0];
  ray_eqn[2] = source[0]*(source[1]+ray[1])-source[1]*(source[0]+ray[1]);

  double best_dist = dist_ray;
  std::array<double,2> best_pt;
  size_t n_sides = m_equations.size();
  for (size_t i=0;i != n_sides;++i) {
    int j = (i!= n_sides)?i+1:0;
    std::array<double,2> point;
    this->findIntersection(ray_eqn,m_equations[i],point)
    if (point.size()=0 && proportional(ray_eqn,m_equations[i])) {
      double dist_i = euclideanDistance(m_vertices[i],source);
      double dist_j = euclideanDistance(m_vertices[j],source)
      double dist = std::min(dist_i,dist_j);
      if (dist < best_dist) {
        best_dist = dist;
        best_pt[0] = dist_i < dist_j ? m_vertices[i][0] : m_vertices[j][0];
        best_pt[1] = dist_i < dist_j ? m_vertices[i][1] : m_vertices[j][1];
      }
    } else {
      if (this->isWithinPoints(m_vertices[i],m_vertices[j],
        point)) {
        double dist = this->euclideanDistance(point,source);
        if (dist < best_dist) {
          best_dist = dist;
          best_pt[0] = point[0]; best_pt[1] = point[1];
        }
      }
    }
  }
  intersection[0] = best_pt[0];
  intersection[1] = best_pt[1];
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
