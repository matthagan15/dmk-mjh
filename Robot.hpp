#ifndef Robot_hpp
#define Robot_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <array>
#include <vector>
#include <map>
#include "World.hpp"
#include "probDist.hpp"

class Robot
{
  World m_world;
  double m_step_size;
  int m_scan_res;
  double m_scan_width;
  double m_scan_acc;
  double m_scan_pow;
  std::array<double,2> m_location;
  double m_direction;
  std::ofstream m_outfile;
  int m_memory; // ranges from 0 (right) to 3 (down)
  probDist m_pd;
public:
  Robot();
  ~Robot();
  void writeToFile(std::vector<std::array<double,2> >& detections);
  void Move();
  void Wander();
  void setStepSize(double step);
  void setScanResolution(int res);
  void setScanWidth(double w);
  void setScanAccuracy(double acc);
  void setScanPower(double power);
  double getStepSize();
  double getScanResolution();
  double getScanWidth();
  double getScanAccuracy();
  double getScanPower();
  double getDirection();
  double getX();
  double getY();
  probDist& getProbDist();
private:
  void Sense(std::vector<std::array<double,2> >&);
};

Robot::Robot(): m_pd(50,50) {
  m_location[0] = 1.0;
  m_location[1] = 1.0;
  m_direction = 6.1;
  this->setScanResolution(10);
  this->setScanAccuracy(1.0);
  this->setScanWidth(2.0*M_PI);
  this->setScanPower(1.5);
  this->setStepSize(0.1);
  m_world.writeToFile();
  m_memory = 1;
  m_outfile.open("robot.txt");
  m_pd.setWidthHeight(m_world.getBounds()[0][1],m_world.getBounds()[1][1]);
  m_pd.setRobotLocation(m_location);
}

Robot::~Robot() {
  m_outfile.close();
}

double Robot::getX() {
  return m_location[0];
}

double Robot::getY() {
  return m_location[1];
}

probDist& Robot::getProbDist() {
  return m_pd;
}

double Robot::getDirection() {
  return m_direction;
}

void Robot::setStepSize(double step) {
  m_step_size = step;
}

void Robot::setScanResolution(int res) {
  m_scan_res = res;
}

void Robot::setScanWidth(double w) {
  m_scan_width = w;
}

void Robot::setScanAccuracy(double acc) {
  m_scan_acc = acc;
}

void Robot::setScanPower(double power) {
  m_scan_pow = power;
}

double Robot::getStepSize() {
  return m_step_size;
}

double Robot::getScanResolution() {
  return m_scan_res;
}

double Robot::getScanWidth() {
  return m_scan_width;
}

double Robot::getScanAccuracy() {
  return m_scan_acc;
}

double Robot::getScanPower() {
  return m_scan_pow;
}

void Robot::Sense(std::vector<std::array<double,2> >& detections) {
  for (int i=0; i!=m_scan_res; ++i) {
    double angle = m_scan_width*i/static_cast<double>(m_scan_res) - m_scan_width/2.0;
    angle += m_direction;
    std::array<double,2> ray;
    ray[0] = m_scan_pow*cos(angle);
    ray[1] = m_scan_pow*sin(angle);
    std::array<double,2> source = {{this->getX(),this->getY()}};
    std::array<double,2> ping;
    bool found = m_world.closestIntersection(ray,source,ping);
    if (found) {
      subtract(ping,source);
      detections.push_back(ping);
    }
  }
}

void Robot::Move() {
  std::vector<std::array<double,2> > detections;
  this->Sense(detections);
  this->writeToFile(detections);
  subtract(detections,m_location);

  size_t num_det = detections.size();
  for (size_t i=0; i != num_det; ++i) {
    double dist = euclideanDistance(m_location,detections[i]);
    if (dist < 3.5*m_step_size) {
      double angle = fmod(getAngle(m_location,detections[i])+2.0*M_PI,2.0*M_PI);
      if (static_cast<int>((angle+M_PI_4)/M_PI_2) == m_memory) {
        m_memory += (randDouble(0.0,1.0)<0.5)? 1 : -1;
        m_memory = (m_memory+4)%4;  //make it from 0 to 3
      }
    }
  }
  if (m_memory%2==0) {
    m_location[0] += (m_memory==0) ? m_step_size:-m_step_size;
  } else {
    m_location[1] += (m_memory==1) ? m_step_size:-m_step_size;
  }
}

void Robot::Wander() {
  for (int i=0; i != 10; ++i) {
    std::vector<std::array<double,2> > detections;
    this->Sense(detections);
    m_pd.update(detections);
    this->writeToFile(detections);
    m_pd.printWallDist();
  }
}


void Robot::writeToFile(std::vector<std::array<double,2> >& detections) {
  m_outfile << "position\n" << this->getX() << "," << this->getY();
  m_outfile << "," << this->getDirection() << "\n";
  m_outfile << "scanner\n" << this->getScanPower() << "," << this->getScanWidth() << "\n";
  size_t size = detections.size();
  m_outfile << "detections\n";
  for (size_t i=0;i!=size;++i) {
    m_outfile << detections[i][0] << "," << detections[i][1] << "\n";
  }
  m_outfile << "end\n";
}

#endif
