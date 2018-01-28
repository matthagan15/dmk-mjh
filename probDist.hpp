#ifndef probDist_hpp
#define probDist_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>

class probDist {
private:
    std::vector<std::vector<double> > grid;
    std::vector<std::vector<double> > wall_grid;
    int grid_width;
    int grid_height;
    std::ofstream m_outfile;
    double world_width;
    double world_height;
    double unit_width;
    double unit_height;
public:
    probDist(int, int);
    probDist();
    ~probDist();
    void initRect(int,int,int,int);
    void setWidthHeight(double,double);
    void printDist();
    void normalizeDist();
    void addProbMass(int,int, double);
    void shiftMass(std::array<double,2>&);
    void shiftMass(int,int);
    void bayesUp(int, int);
    std::array<int,2> gridify(const std::array<double,2>&);
    std::vector<std::array<int,2> > gridify(const std::vector<std::array<double,2> >&);
    void update(std::vector<std::array<double,2> >&, std::vector<std::array<double,2> >&);
    void printWallDist();
    std::vector<std::array<int,2> > getIntersectionsDetection(int x, int y, double slope, double angle);
    void writeToFile(std::ofstream& outfile);
    std::array<int,2> getClosestGridPoint(double , double );
    void bayesUpOne(int,int,bool,double);
    void bayesUpBatch(std::vector<std::array<int,2>>);
    void setRobotLocation(std::array<double,2>& loc);
};

probDist::probDist(int m, int n) {
    grid_width = m;
    grid_height = n;
    const double init_prob = 0.8;
    for (int i=0;i<m;i++) {
        std::vector<double> v(n);
        std::vector<double> w(n,init_prob);
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

void probDist::setRobotLocation(std::array<double,2>& loc) {
    int x;
    int y;
    x = std::round(loc[0]/this->unit_width);
    y = std::round(loc[1]/this->unit_height);
    this->addProbMass(x,y,1.0);
}

void probDist::setWidthHeight(double map_width, double map_height) {
    this->world_width = map_width;
    this->world_height = map_height;
    this->unit_width = map_width/(1.0*this->grid_width);
    this->unit_height = map_height/(1.0*this->grid_height);
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
            std::cout << std::setprecision(4) << 1.0*wall_grid[i][j] << " | ";
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

void probDist::shiftMass(std::array<double,2>& shift) {
  std::array<int,2> int_shift = probDist::gridify(shift);
  this->shiftMass(int_shift[0],int_shift[1]);
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

void probDist::bayesUpOne(int x, int y, bool hit, double prob) {
    double pObsGivenWall = 0.95;
    double pObsGivenNoWall = 0.01;
    double pNoObsGivenWall = 0.05;
    double pNoObsGivenNoWall = 0.99;
    double post;
    double num;
    double den;
    // double offset;

    if (hit) {
        // std::cout << "Detection here: " << x << "," << y << "\n";
        num = (pObsGivenWall*wall_grid[x][y]);
        den = pObsGivenWall*wall_grid[x][y] + pObsGivenNoWall * (1-wall_grid[x][y]);
        post = num/den;
        post *= prob;
        wall_grid[x][y] = post;
    } else {
        // std::cout << "No Detection here: " << x << "," << y << "\n";
        num = pNoObsGivenNoWall * (1.0 - wall_grid[x][y]);
        den = num + pNoObsGivenWall * wall_grid[x][y];
        post = (num/den);
        // Sets to 1-posterior because is calculating P[no wall | no obs] = 1 - P[wall | no obs]
        wall_grid[x][y] = prob*(1-post);
    }
}



void probDist::bayesUp(int dx,int dy) {
    double distance = std::pow(std::pow(dx,2) + std::pow(dy,2),0.5);
    double slope = std::atan2(dy,dx);

    // std::cout << "dx: " << dx << "\n";
    // std::cout << "dy: " << dy << "\n";
    // std::cout << "Slope: " << slope << "\n";
    // std::cout << "Dist: " << distance << "\n";
    int x;
    int y;
    bool bayesHit;
    std::array<int,2> detectPt;
    std::vector<std::array<int,2>> grid_pts;

    for (int i=0; i != grid_width; i++) {
        for (int j = 0; j != grid_height; j++) {
            // This is going through entire grid. Idea is to process a detection at each point.

            // TODO: Update robot location probability given a detection of certain distance from wall. If you
            // observer something two points away, there is no way you are at edge of map.
            if (i + dx < 0 || i + dx >= grid_width || j+ dy <0 || j+dy >= grid_height) {continue;}
            if (grid[i][j] < 1e-4) {continue;}

            // std::cout << "Bayes up is currently at: " << i << "," << j << "\n";
            grid_pts = this->getIntersectionsDetection(i,j,slope,distance);
            detectPt = this->getClosestGridPoint(i+dx,j+dy);

            // std::cout << "Detect point: " << detectPt[0] << "," << detectPt[1] << "\n";

            // This if puts detection pt in grid_pts vector if it is not.
            if (std::find(grid_pts.begin(),grid_pts.end(), detectPt) == grid_pts.end()) {grid_pts.push_back(detectPt);}
            for (int k=0; k != grid_pts.size();k++) {
                // std::cout << "Detection line point" << k <<": " << grid_pts[k][0] << "," << grid_pts[k][1] << "\n";
            }
            for (int k=0; k != grid_pts.size();k++) {
                // P[no wall| no obs] = P[No obs | no wall] * P[no wall] / (p[no obs|no wall]*p[no wall] + p[no obs|wall] * p[wall])
                x = grid_pts[k][0];
                y = grid_pts[k][1];
                if (x >= grid_width || x < 0 || y < 0 || y >= grid_height) continue;

                // std::cout << "Bayes update at: " << grid_pts[k][0] << "," << grid_pts[k][1] << "\n";
                // Updates bayesHit boolean to give whether current line of sight is the wall hit or not.
                bayesHit =  (x == detectPt[0] && y == detectPt[1]) ? true : false;
                this->bayesUpOne(x,y,bayesHit,grid[i][j]);
            }

        }
    }
}

void probDist::bayesUpBatch(std::vector<std::array<int,2>> v) {
    std::array<int,2> pt;
    for (int i=0; i != v.size();i++) {
        pt = v[i];
        // std::cout << "Point update: " << pt[0] << "," << pt[1] << "\n";
        this->bayesUp(pt[0],pt[1]);
    }
}

std::vector<std::array<int,2> > probDist::getIntersectionsDetection(int x, int y, double slope, double distance) {
    std::vector<std::array<int,2> > v;
    double curr_dist = 0.0;
    double dx;
    double dy;

    double theta = slope;
    dx = std::cos(theta)*0.1;
    dy = std::sin(theta)*0.1;

    double px = 1.0*x;
    double py = 1.0*y;
    double dz = std::pow(std::pow(dx,2) + std::pow(dy,2),0.5);
    std::array<int,2> grid_pt = {{x,y}};
    v.push_back(grid_pt);
    // std::cout << "************************************************\nPrinting out getIntersectionsDetection\n";
    while(curr_dist <= distance) {
        px = px + dx;
        py = py + dy;
        // std::cout << "px,py: " << px << ", " << py << "\n";
        curr_dist += dz;
        grid_pt = this->getClosestGridPoint(px,py);
        // std::cout << "grid_pt: " << grid_pt[0] << "," << grid_pt[1] << "\n";
        if(std::find(v.begin(), v.end(), grid_pt) != v.end()) {
            continue;
        } else {
            v.push_back(grid_pt);
        }
    }
    return v;
}

std::array<int,2> probDist::gridify(const std::array<double,2>& pt) {
  double x = pt[0]/this->unit_width;
  double y = pt[1]/this->unit_height;
  int grid_x = std::floor(x);
  int grid_y = std::floor(y);
  std::array<int,2> ret = {{grid_x,grid_y}};
  return ret;
}


std::vector<std::array<int,2> > probDist::gridify(const std::vector<std::array<double,2> >& info) {
    std::vector<std::array<int,2 > > outputVec;
    size_t size = info.size();
    outputVec.reserve(size);
    for (size_t i=0;i != size;i++) {
        std::array<int,2> new_pt = probDist::gridify(info[i]);
        outputVec.push_back(new_pt);
    }
    return outputVec;
}

void probDist::update(std::vector<std::array<double,2> >& det,
                        std::vector<std::array<double,2> >& ndet) {
    std::vector<std::array<int,2>> grid_det = probDist::gridify(det);
    std::vector<std::array<int,2>> grid_ndet = probDist::gridify(ndet);
    probDist::bayesUpBatch(grid_det,true);
    probDist::bayesUpBatch(grid_ndet,false);
}

std::array<int,2> probDist::getClosestGridPoint(double x, double y) {
    std::array<int,2> returner;
    int xpt = std::round(x);
    int ypt = std::round(y);
    returner[0] = xpt;
    returner[1] = ypt;
    return returner;
}

void probDist::writeToFile(std::ofstream& outfile) {
  outfile << "wall_dist\n";
  for (int j = grid_height-1; j != -1; j--)  {
      for (int i = 0; i != grid_width; i++){
          outfile << wall_grid[i][j];
          if (i != grid_width-1) {
            outfile << ",";
          }
      }
      outfile << "\n";
  }
  outfile << "robot_dist\n";
  for (int j = grid_height-1; j != -1; j--)  {
      for (int i = 0; i != grid_width; i++){
          outfile << grid[i][j];
          if (i != grid_width-1) {
            outfile << ",";
          }
      }
      outfile << "\n";
  }
}

#endif
