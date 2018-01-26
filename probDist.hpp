#ifndef probDist_hpp
#define probDist_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

class probDist {
private:
    std::vector<std::vector<double> > grid;
    std::vector<std::vector<double> > wall_grid;
    int grid_width;
    int grid_height;
    std::ofstream m_outfile;
public:
    probDist(int, int);
    probDist();
    ~probDist();
    void initRect(int,int,int,int);
    void printDist();
    void normalizeDist();
    void addProbMass(int,int, double);
    void shiftMass(int,int);
    void bayesUp(int, int);

    void printWallDist();
    std::vector<std::array<int,2> > getIntersectionsDetection(int x, int y, double slope, double angle);
    void writeToFile();
    std::array<int,2> getClosestGridPoint(double , double );
};

probDist::probDist(int m, int n) {
    grid_width = m;
    grid_height = n;
    for (int i=0;i<m;i++) {
        std::vector<double> v(n);
        std::vector<double> w(n,0.01);
        grid.push_back(v);
        wall_grid.push_back(w);
    }
    m_outfile.open("probDist.txt");
}

probDist::probDist() {
    std::vector<double> v(0);
    grid.push_back(v);
}

probDist::~probDist() {
  m_outfile.close();
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
    for (int j = grid_height -1; j != -1; j--)  {
        std::cout << "row " << j << ": ";
        for (int i = 0; i != grid_width; i++){
            std::cout << grid[i][j] << " | ";
        }
        std::cout << "\n_________________________________\n";
    }
}

void probDist::printWallDist() {
    std::cout << "Printing Wall Distribution. \n ***********************************\n";
    for (int j = grid_height -1; j != -1; j--)  {
        std::cout << "row " << j << ": ";
        for (int i = 0; i != grid_width; i++){
            std::cout << wall_grid[i][j] << " | ";
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
}

void probDist::shiftMass(int horiz, int vert) {
    if (horiz > 0) {
        for (int i=grid_width -2; i!= -1; i--) {
            for (int j = 0; j != grid_height; j++) {
                int dest = (i+horiz) >= grid_width -1 ? grid_width - 1 : i + horiz;
                this->addProbMass(dest,j,grid[i][j]);
                if (i != grid_width - 1) {grid[i][j] = 0.0;}
            }
        }
    } else if (horiz < 0) {
        for (int i=1; i!=grid_width; i++) {
            for (int j=0; j!= grid_height; j++) {
                int dest = (i + horiz) <= 0 ? 0 : i + horiz;
                this->addProbMass(dest,j,grid[i][j]);
                if (i != 0) {grid[i][j] = 0.0;}
            }
        }
    }
    if (vert > 0) {
        for (int i=0; i!= grid_width; i++) {
            for (int j = grid_height - 2; j != -1; j--) {
                int dest = (j+vert) >= grid_height -1 ? grid_height - 1 : j + vert;
                std::cout << "Coords:" << i << "," << j <<"\n";
                std::cout << "Mass: " << grid[i][j] << "\n";
                std::cout << "Dest:" << dest << "\n";
                this->addProbMass(i,dest,grid[i][j]);
                std::cout << "Mass at dest: " << grid[i][dest] << "\n";
                if(j != grid_height - 1) {grid[i][j] = 0.0;}
            }
        }
    } else if (vert < 0) {
        for (int i=0; i!=grid_width; i++) {
            for (int j=1; j!= grid_height; j++) {
                int dest = (j + vert) <= 0 ? 0 : j + vert;
                this->addProbMass(i,dest,grid[i][j]);
                if (j!= 0) {grid[i][j] = 0.0;}
            }
        }
    }
}



void probDist::bayesUp(int dx,int dy) {
    // P[wall | obs] = P[obs | wall ] * p[wall] / \sum (P[obs | no wall] * P[no wall] + P[obs | wall] * P[wall])
    double pObsGivenWall = 0.95;
    double pObsGivenNoWall = 0.01;
    double pNoObsGivenWall = 0.05;
    double pNoObsGivenNoWall = 0.99;
    double post;
    double num;
    double den;
    double distance = std::pow(std::pow(dx,2) + std::pow(dy,2),0.5);
    double slope = dy/(1.0*dx);
    int x;
    int y;
    std::array<int,2> detectPt;
    std::vector<std::array<int,2>> grid_pts;

    for (int i=0; i != grid_width; i++) {
        for (int j = 0; j != grid_height; j++) {
            if (i + dx < 0 || i + dx >= grid_width || j+ dy <0 || j+dy >= grid_height) {continue;}
            if (grid[i][j] < 1e-10) {continue;}

            grid_pts = this->getIntersectionsDetection(i,j,slope,distance);

            detectPt = this->getClosestGridPoint(i+dx,j+dy);
            std::cout << "Detect point: " << detectPt[0] << "," << detectPt[1] << "\n";
            if (std::find(grid_pts.begin(),grid_pts.end(), detectPt) == grid_pts.end()) {grid_pts.push_back(detectPt);}
            for (int k=0; k != grid_pts.size();k++) {

                // P[no wall| no obs] = P[No obs | no wall] * P[no wall] / (p[no obs|no wall]*p[no wall] + p[no obs|wall] * p[wall])
                x = grid_pts[k][0];
                y = grid_pts[k][1];

                std::cout << "Printing Bayes up grids along detection line. \n";
                for (int l=0; l != grid_pts.size();l++) {
                    std::cout << "Pt: " << grid_pts[l][0] << "," << grid_pts[l][1] << "\n";
                }

                if (x >= grid_width || x < 0 || y < 0 || y >= grid_height) continue;
                if (x != detectPt[0] && y != detectPt[1]) {
                    std::cout << "No Detection here: " << x << "," << y << "\n";
                    num = pNoObsGivenNoWall * (1.0 - wall_grid[x][y]);
                    den = num + pNoObsGivenWall * wall_grid[x][y];
                    post = (num/den)*grid[i][j];
                } else {
                    std::cout << "Detection here: " << x << "," << y << "\n";
                    x = grid_pts[k][0];
                    y = grid_pts[k][1];
                    post = (pObsGivenWall*wall_grid[x][y])/(pObsGivenWall*wall_grid[x][y] + pObsGivenNoWall * (1-wall_grid[x][y]));
                    post *= grid[i][j];
                    wall_grid[x][y] = post;
                }
            }

        }
    }
}

std::vector<std::array<int,2> > probDist::getIntersectionsDetection(int x, int y, double slope, double distance) {
    std::vector<std::array<int,2> > v;
    double curr_dist = 0.0;
    double dx = 0.1;
    double dy = slope*dx;
    double px = 1.0*x;
    double py = 1.0*y;
    double dz = std::pow(std::pow(dx,2) + std::pow(dy,2),0.5);
    std::array<int,2> grid_pt = {{x,y}};
    v.push_back(grid_pt);
    while(curr_dist <= distance) {
        px = px + dx;
        py = py + dy;
        curr_dist += dz;
        grid_pt = this->getClosestGridPoint(px,py);
        if(std::find(v.begin(), v.end(), grid_pt) != v.end()) {
            continue;
        } else {
            v.push_back(grid_pt);
        }
    }
    return v;
}


std::array<int,2> probDist::getClosestGridPoint(double x, double y) {
    std::array<int,2> returner;
    int xpt = std::round(x);
    int ypt = std::round(y);
    returner[0] = xpt;
    returner[1] = ypt;
    return returner;
}

void probDist::writeToFile() {
  for (int j = grid_height-1; j != -1; j--)  {
      for (int i = 0; i != grid_width; i++){
          m_outfile << grid[i][j];
          if (i != grid_width-1) {
            m_outfile << ",";
          }
      }
      m_outfile << "\n";
  }
  m_outfile << "next\n";
}

#endif
