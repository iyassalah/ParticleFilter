
#ifndef AI_PARTICLE_FILTER_ROBOT_H
#define AI_PARTICLE_FILTER_ROBOT_H

#include "Sensor.h"
#include <vector>

using namespace std;
class Robot {
public:
    Robot() {

    }

    Robot(int startPos, vector<double> sensorReads) : sensor(sensorReads) {
        this->startPosition = startPos;
        position = this->startPosition;
    }

    Robot(Robot *pRobot) {

    }

    void move() {
        this->position++;
    }

    int getRobotPosition() {
        return this->position;
    }

    int getStartPosition() {
        return this->startPosition;
    }

    Sensor sensor;
private:
    int startPosition;
    int position;
};


#endif //AI_PARTICLE_FILTER_ROBOT_H
