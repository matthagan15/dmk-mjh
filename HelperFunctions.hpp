#ifndef HelperFunctions_hpp
#define HelperFunctions_hpp

#include <stdio.h>
#include <cmath>
#include <algorithm>
#include <random>

template<class T>
double euclideanDistance(const T& pointA,const T& pointB) {
    return pow(pow(pointA[0]-pointB[0],2.0)+pow(pointA[1]-pointB[1],2.0),0.5);
  }


bool equal(const double a, const double b, double epsilon=0.000001) {
  return abs(a-b) < epsilon;
}

template<class T>
double getAngle(const T& v1, const T& v2) {
  if (equal(v1[0],v2[0])) {
    return v2[1] > v1[1] ? M_PI_2 : -M_PI_2;
  }
  return atan2((v2[1]-v1[1]),(v2[0]-v1[0]));
}

template<class T>
double getAngle(const T& v1) {
  if (equal(v1[0],0.0)) {
    return v1[1] > 0.0 ? M_PI_2 : -M_PI_2;
  }
  return atan2(v1[1],v1[0]);
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

double randDouble(double low, double high)
{
    double temp;
    /* swap low & high around if the user makes no sense */
    if (low > high)
    {
        temp = low;
        low = high;
        high = temp;
    }

    /* calculate the random number & return it */
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<float> dis(0.0,1.0);

    temp = dis(gen)*(high-low) + low;
    return temp;
}


double limit(const double x, const double low, const double high) {
  if (x < low) {
    return low;
  }
  if (x > high) {
    return high;
  }
  return x;
}

template<class T>
void subtract(T& vecA,const T& vecB) {
  // subtract B from A
  size_t size = vecB.size();
  for (size_t i=0; i!=size;++i) {
    vecA[i] -= vecB[i];
  }
}

template<class T>
void subtract(std::vector<T>& A, const T& B) {
  // subtract B from A
  typedef typename std::vector<T>::iterator iter;
  for (iter i=A.begin();i!=A.end();++i) {
    subtract(*i,B);
  }
}



#endif
