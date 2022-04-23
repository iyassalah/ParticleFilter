
#ifndef AI_PARTICLE_FILTER_HELPERS_H
#define AI_PARTICLE_FILTER_HELPERS_H

#include <cmath>
#include <iostream>
#include <vector>

#include "Particle.h"
using namespace std;

double calculateStandardDeviation(vector<double> reads, double mean) {
    double sum = 0;
    for (int i = 0; i < 1000; i++) {
        sum += pow(reads[i] - mean, 2);
    }
    return sum / 1000;
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
        Particle temp = *new Particle((double)1 / 50, curPos);
        particles.push_back(temp);
        curPos += skipDis;
    }
}

#endif //AI_PARTICLE_FILTER_HELPERS_H


