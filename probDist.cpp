#include "probDist.hpp"

probDist::probDist(int m, int n) {
    for (int i=0;i<m;i++) {
        vector<double> v(n);
        grid.push_back(v);
    }
}
