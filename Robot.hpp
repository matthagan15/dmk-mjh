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
  double m_scan_width;
  double m_scan_acc;
  double m_scan_pow;
  std::array<double,2> m_location;
  double m_direction;
  std::ofstream m_outfile;
  double m_memory; // ranges from -1 (left) to 1 (right)
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
private:
  void Sense(std::vector<std::array<double,2> >&,std::vector<std::array<double,2> >&);
};

Robot::Robot() {
  m_location[0] = 1.0;
  m_location[1] = 8.0;
  m_direction = M_PI_2;
  this->setScanResolution(20);
  this->setScanAccuracy(0.2);
  this->setScanWidth(0.3);
  this->setScanPower(2.0);
  this->setStepSize(0.2);
  m_world.writeToFile();
  m_memory = 0.0;
  m_outfile.open("robot.txt");
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

void Robot::Sense(std::vector<std::array<double,2> >& l_detections,
  std::vector<std::array<double,2> >& r_detections) {
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
      // ping[0] += randDouble(-m_scan_acc,m_scan_acc);
      // ping[1] += randDouble(-m_scan_acc,m_scan_acc);
      if (angle > m_direction) {
        l_detections.push_back(ping);
      } else {
        r_detections.push_back(ping);
      }
    }
  }
}

void Robot::Move() {
  std::vector<std::array<double,2> > l_detections;
  std::vector<std::array<double,2> > r_detections;
  this->Sense(l_detections,r_detections);
  l_detections.insert(l_detections.end(), r_detections.begin(), r_detections.end());
  this->writeToFile(l_detections);

  std::array<double,2> l = {{l_detections.front()[0],l_detections.front()[1]}};
  std::array<double,2> r = {{l_detections.back()[0],l_detections.back()[1]}};
  double r_angle = angle(m_location,r);
  double l_angle = angle(m_location,l);
  double avg_angle = (l_angle+r_angle)/2.0 - m_direction;
  m_memory += avg_angle*0.1;
  m_memory = limit(m_memory,-1.0,1.0);
  if (m_memory > 0.1) {
    m_direction -= 0.1;
  } else if (m_memory < -0.1) {
    m_direction += 0.1;
  }
  m_location[0] += m_step_size*cos(m_direction);
  m_location[1] += m_step_size*sin(m_direction);
}

void Robot::Wander() {
  for (int i=0; i != 10; ++i) {
    this->Move();
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
