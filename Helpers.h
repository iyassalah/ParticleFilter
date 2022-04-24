
#ifndef AI_PARTICLE_FILTER_HELPERS_H
#define AI_PARTICLE_FILTER_HELPERS_H

#include <cmath>
#include <iostream>
#include <vector>

#include "Particle.h"
#include "Robot.h"
using namespace std;



double calculateStandardDeviation(vector<double> reads, double mean) {
    double sum = 0;
    for (int i = 0; i < 1000; i++) {
        sum += pow(reads[i] - mean, 2);
    }
    return sqrt(sum / 1000);
}

double getPositionsRead(vector<double> &positions) {
    double sum = 0;
    int j = 0;
    for (double i = 0; i < 1; i += 0.001) {
        positions[j] = cos(i) + 0.5 * cos(3 * i + 0.23) + 0.5 * cos(5 * i - 0.4) + 0.5 * cos(7 * i + 2.09) + 0.5 * cos(9 * i - 3);
        sum += positions[j];
        j++;
    }
    return  sqrt(sum / 1000);
}

void getInitialDistribution(vector<Particle> &particles, int skipDis, int numOfParticles) {
    int curPos = 0;
    for (int i = 0; i < numOfParticles; i++) {
        Particle temp = Particle((double)1 / 50, curPos);
        particles.push_back(temp);
        curPos += skipDis;
    }
}

Robot createRobot(vector<double>reads) {
    while (true) {
        cout << "Please enter the initial position of the Robot (must be between 1 and 1000) : ";
        int pos;
        cin >> pos;
        if (pos >= 1 and pos <= 1000) {
            return Robot(pos, reads);
        }
        else {
            cout << "invalid input try again\n";
        }
    }
}

double weight(double x, double u, double q) {
    const double PI = acos(-1.0);
    return ((1 / (q * sqrt(2 * PI))) * exp(-1 * pow((x - u), 2) / 2 * pow(q, 2)));
}

unsigned long long generateRandom(unsigned long long mod) {
    return rand() % mod;
}

void calcNewWeights(vector<Particle> &particles, vector<double>posRead, Robot robot, double mean, double SD) {
    double sum = 0;
    vector<Particle> newParicles;

    ///resampling particles
    for (int i = 0; i < particles.size(); i++) {
        Particle temp = new Particle();
        temp.setPosition(generateRandom(999));
        temp.setWeight(weight((posRead[i] - posRead[temp.getPosition()]), mean, SD));
        sum += temp.getWeight();
        newParicles.push_back(temp);
    }

    for (int i = 0; i < particles.size(); i++) {
        // to normalize the weights for all particles
        newParicles[i].setWeight(newParicles[i].getWeight() / sum);

        //replace the old particles with new ones
        particles[i] = newParicles[i];
    }
}

#endif //AI_PARTICLE_FILTER_HELPERS_H


