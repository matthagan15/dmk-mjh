#ifndef probDist_hpp
#define probDist_hpp

#include <iostream>
#include <stdio.h>
#include <vector>

class probDist {
private:
    std::vector<std::vector<double> > grid;
    int grid_width;
    int grid_height;
public:
    probDist(int, int);
    probDist();
    void initRect(int,int,int,int);
    void printDist();
    void normalizeDist();
    void addProbMass(int,int, double);
    void shiftMass(int,int);
};

probDist::probDist(int m, int n) {
    grid_width = m;
    grid_height = n;
    for (int i=0;i<m;i++) {
        std::vector<double> v(n);
        grid.push_back(v);
    }
}

probDist::probDist() {
    std::vector<double> v(0);
    grid.push_back(v);
}

void probDist::initRect(int x1, int y1, int x2, int y2) {
    /* Initializes distribution to a uniform rectangle
    */
    if (x2 > grid_width - 1 || x1 > grid_width || y2 > grid_height - 1 || y1 > grid_height) {
        return;
    }
    if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0) {
        return;
    }

    int area = (x2 - x1 + 1) * (y2 - y1 + 1);
    if (x1 >= x2 && y2 >= y1) {
            area *= -1;
    } else if (y1 >= y2 && x1 <= x2) {
        area *= -1;
    }
    int width = x2 - x1 + 1;
    int height = y2 - y1 + 1 ;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            grid[x1 + i][y1 + j] = 1.0/area;
        }
    }
}

void probDist::printDist() {
    std::cout << "Printing Distribution. \n ***********************************\n";
    for (int i = grid_width - 1; i >= 0; i--) {
        std::cout << "row " << i << ": ";
        for (int j = 0; j < grid_height; j++) {
            std::cout << grid[i][j] << " | ";
        }
        std::cout << "\n_________________________________\n";
    }
}

void probDist::normalizeDist() {
    double tot = 0.0;
    for (int i = 0; i != grid_width; i++) {
        for (int j = 0; j != grid_height; j++) {
            tot += grid[i][j];
        }
    }
    for (int i = 0; i != grid_width; i++) {
        for (int j = 0; j != grid_height; j++) {
            grid[i][j] *= 1.0/tot;
        }
    }

}

void probDist::addProbMass(int x, int y, double mass) {
    grid[x][y] += mass;
    this->normalizeDist();
}

// void probDist::shiftMass(int horiz, int vert) {
//
// }

#endif
