#include <stdio.h>
#include <iostream>
#include "World.hpp"
#include "Robot.hpp"
#include "probDist.hpp"
#include <iomanip>

int main() {
    /*
    Gameplan:
        *Overall goal is to build a HMM controller to navigate a robot through an obstacle course.
        *Need to build:
            -Probability handler. Something to store a distribution and update it. Could be done in HMM.
            -An HMM class. Do we want this separate and more general, or very integrated into the robot
            -Robot class, should basically be a wrapper/storage for holding location and data,
            as well as updating location and issuing commands.
    */
    Robot robot;
    robot.Wander();
    // probDist pd(20,20);
    // pd.setWidthHeight(2.0,2.0);
    // std::array<double,2> init_loc = {{1.0,1.0}};
    // pd.setRobotLocation(init_loc);
    // // pd.initRect(0,0,1,1);
    // // pd.addProbMass(4,4,0.5);
    // // pd.addProbMass(0,4,0.5);
    // // pd.addProbMass(1,0,0.5);
    // std::vector<std::array<int,2> > grid_pts = pd.getIntersectionsDetection(0,0,1.0/3.0,10);
    // std::cout << "Grid points size: " << grid_pts.size() << "\n";
    // for (int i = 0; i != grid_pts.size(); i++) {
    //     std::cout << "Grid pt: " << grid_pts[i][0] << ","<<grid_pts[i][1] << "\n";
    // }
    // pd.printDist();
    // // pd.printWallDist();
    // // pd.bayesUp(2,2);
    // // pd.printWallDist();
    // // pd.bayesUp(2,2);
    // pd.printWallDist();
    // std::vector<std::array<double,2>> v;
    // v.push_back(std::array<double,2> {{0.5,0.5}});
    // v.push_back(std::array<double,2> {{0.5,-0.5}});
    // v.push_back(std::array<double,2> {{-0.5,0.5}});
    // v.push_back(std::array<double,2> {{-0.5,-0.5}});
    // v.push_back(std::array<double,2> {{0.5,0.5}});
    // v.push_back(std::array<double,2> {{0.5,-0.5}});
    // v.push_back(std::array<double,2> {{-0.5,0.5}});
    // v.push_back(std::array<double,2> {{-0.5,-0.5}});
    //
    //
    // pd.update(v);
    // pd.printWallDist();

}
