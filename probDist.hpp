#ifndef probDist_hpp
#define probDist_hpp

#include <iostream>
#include <stdio.h>
#include <vector>

class probDist {
private:
    std::vector<std::vector<double> > grid;
public:
    probDist(int n, int m);
};


#endif
