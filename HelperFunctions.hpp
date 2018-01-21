#ifndef HelperFunctions_hpp
#define HelperFunctions_hpp

#include <stdio.h>
#include <cmath>
#include <algorithm>

template<class T>
double euclideanDistance(const T& pointA,const T& pointB) {
    return pow(pow(pointA[0]-pointB[0],2.0)+pow(pointA[1]-pointB[1],2.0),0.5);
  }

template<class T>
bool equal(const T a, const T b, double epsilon=0.000001) {
  return abs(a-b) < epsilon;
}

template<class T>
bool proportional(const T& a, const T& b) {
  size_t size = a.size();
  if (size != b.size()) {
    std::cout << "Not same size\n";
    return false;
  }
  double constant = a[0]/static_cast<double>(b[0]);
  for (size_t i=1; i!= size;++i) {
    if (!equal(a[i],constant*b[i])) {
      return false;
    }
  }
  return true;
}

#endif
