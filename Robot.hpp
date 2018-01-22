#ifndef Robot_hpp
#define Robot_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <array>
#include <vector>
#include "World.hpp"

class Robot
{
  World m_world;
  double m_step_size;
  int m_scan_res;
  double m_scan_acc;
  double m_scan_pow;
  std::array<double,2> m_location;
public:
  Robot();
  void writeToFile(std::vector<std::array<double,2> >& detections);
  void Move();
  void setStepSize(double step);
  void setScanResolution(int res);
  void setScanAccuracy(double acc);
  void setScanPower(double power);
  double getX();
  double getY();
private:
  void Sense(std::vector<std::array<double,2> >& detections);
};

Robot::Robot() {
  m_location[0] = 4.0;
  m_location[1] = 4.0;
  this->setScanResolution(30);
  this->setScanAccuracy(0.2);
  this->setScanPower(5.0);
  this->setStepSize(0.2);
  m_world.writeToFile();
}

double Robot::getX() {
  return m_location[0];
}

double Robot::getY() {
  return m_location[1];
}

void Robot::setStepSize(double step) {
  m_step_size = step;
}

void Robot::setScanResolution(int res) {
  m_scan_res = res;
}

void Robot::setScanAccuracy(double acc) {
  m_scan_acc = acc;
}

void Robot::setScanPower(double power) {
  m_scan_pow = power;
}

void Robot::Sense(std::vector<std::array<double,2> >& detections) {
  for (int i=0; i!=m_scan_res; ++i) {
    double angle = 2*M_PI*i/static_cast<double>(m_scan_res);
    std::array<double,2> ray;
    ray[0] = m_scan_pow*cos(angle);
    ray[1] = m_scan_pow*sin(angle);
    std::array<double,2> source = {{this->getX(),this->getY()}};
    std::cout << "ray: " << ray[0] << "," << ray[1] << "\n";
    std::cout << "source: " << source[0] << "," << source[1] << "\n";
    std::array<double,2> ping;
    bool found = m_world.closestIntersection(ray,source,ping);
    if (found) {
      ping[0] += randDouble(-m_scan_acc,m_scan_acc);
      ping[1] += randDouble(-m_scan_acc,m_scan_acc);
      detections.push_back(ping);
    }
  }
}

void Robot::Move() {
  std::vector<std::array<double,2> > detections;
  this->Sense(detections);
  this->writeToFile(detections);
}

void Robot::writeToFile(std::vector<std::array<double,2> >& detections) {
  std::fstream outfile("robot.txt",std::fstream::out);
  outfile << "position\n" << this->getX() << "," << this->getY() << "\n";

  size_t size = detections.size();
  outfile << "detections\n";
  for (size_t i=0;i!=size;++i) {
    outfile << detections[i][0] << "," << detections[i][1] << "\n";
  }
  outfile.close();
}

#endif
