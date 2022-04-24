#include <iostream>
#include <vector>
#include <cmath>

#include "Particle.h"
#include "Helpers.h"
#include "Robot.h"
using namespace std;


int main() {
    const int PARTICLES_NUMBER = 50;
    const int CORRIDOR_LENGTH = 1000;

    //list of parti[
    vector<Particle> particles;


    //list of size 100 that holds the sensor read for each position in the corridor
    vector<double> positionsRead(CORRIDOR_LENGTH, 0);

    //calculate the mean value for all positions
    double mean = getPositionsRead(positionsRead);

    double standardDeviation = calculateStandardDeviation(positionsRead, mean);

    //skip distance determine the initial distance between the particle in the first distribution
    int skipDistance = CORRIDOR_LENGTH / PARTICLES_NUMBER;

    // function that gives all particles the first position with fixed distance between each other
    getInitialDistribution(particles, skipDistance, PARTICLES_NUMBER);

    //return a robot with start position read from console
    Robot robot = createRobot(positionsRead);

    ///Resampling particles
    calcNewWeights(particles, positionsRead, robot, mean, standardDeviation);

    moveInCorridor(robot, particles, positionsRead, mean, standardDeviation);
    cout << 1 << endl;
    return 0;
}
