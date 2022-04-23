
#ifndef AI_PARTICLE_FILTER_ROBOT_H
#define AI_PARTICLE_FILTER_ROBOT_H


class Robot {
public:
    Robot() {

    }

    Robot(int startPositon, double sensorRead, int size) {
        postion = this->startPostion;
    } ///must add a sensor constractor

    Robot(Robot *pRobot) {

    }

    void move() {
        this->postion++;
    }

    int getRobotPostion() {
        return this->postion;
    }

private:
    int startPostion;
    int postion;
    /// add sensor
};


#endif //AI_PARTICLE_FILTER_ROBOT_H
