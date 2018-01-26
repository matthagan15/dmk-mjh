#include <stdio.h>
#include <iostream>
#include "World.hpp"
#include "Robot.hpp"
#include "probDist.hpp"

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
    probDist pd(5,5);
    // pd.initRect(0,0,1,1);
    pd.addProbMass(0,0,1.0);
    std::vector<std::array<int,2> > grid_pts = pd.getIntersectionsDetection(0,0,1.0/3.0,10);
    std::cout << "Grid points size: " << grid_pts.size() << "\n";
    for (int i = 0; i != grid_pts.size(); i++) {
        std::cout << "Grid pt: " << grid_pts[i][0] << ","<<grid_pts[i][1] << "\n";
    }
    pd.printDist();
    pd.printWallDist();
    pd.bayesUp(0,4);
    pd.printWallDist();
    pd.bayesUp(0,4);
    pd.printWallDist();
    // pd.writeToFile();
    // pd.shiftMass(20,0);
    // pd.writeToFile();
    // pd.shiftMass(0,-20);
    // pd.writeToFile();
    // pd.shiftMass(0,20);
    // pd.writeToFile();
    // pd.shiftMass(-20,0);
    // pd.writeToFile();
    // pd.shiftMass(0,-10);
    // pd.writeToFile();
    // pd.shiftMass(0,-10);
    // pd.writeToFile();
    // pd.bayesUp(1,1);
    // pd.bayesUp(1,1);
    // pd.bayesUp(1,1);

}
