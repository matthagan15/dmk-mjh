#include <stdio.h>
#include <iostream>
#include "World.hpp"
#include "Robot.hpp"

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
    std::cout << "Do you know da wae" << '\n';
    std::vector<std::array<double,2> > verts = {{{5.0,5.0}},{{6.0,5.0}},{{6.0,6.0}},{{5.0,7.0}}};
    Polygon poly(verts);
    std::cout << "num eqns: " << poly.m_equations.size() << "\n";
    for (int i = 0; i != 4; ++i) {
      std::cout << "(" << poly.m_vertices[i][0] << "," << poly.m_vertices[i][1] << ") ";
    }
    std::cout << "\n";
    for (int i = 0; i != 4; ++i) {
      std::cout << poly.m_equations[i][0] << "," << poly.m_equations[i][1] << "," << poly.m_equations[i][2] << "\n";
    }
    // Robot robot;
    // robot.Move();
}
