
#ifndef AI_PARTICLE_FILTER_HELPERS_H
#define AI_PARTICLE_FILTER_HELPERS_H

#include <cmath>
#include <iostream>
#include <vector>
#include <random>
#include <functional>
#include <random>
#include <set>

#include "Particle.h"
#include "Robot.h"
using namespace std;



double calculateStandardDeviation(vector<double> reads, double mean, int len) {
    double sum = 0;
    for (int i = 0; i < len; i++) {
        sum += pow(reads[i] - mean, 2);
    }
    return sqrt(sum / len);
}

double getPositionsRead(vector<double> &positions, int len) {
    double sum = 0;
    int j = 0;
    for (double i = 0; i < 1; i += 0.001) {
        positions[j] = cos(i) + 0.5 * cos(3 * i + 0.23) + 0.5 * cos(5 * i - 0.4) + 0.5 * cos(7 * i + 2.09) + 0.5 * cos(9 * i - 3);
        sum += positions[j];
        j++;
    }
    return  sum / len;
}

void getInitialDistribution(vector<Particle> &particles, int skipDis, int numOfParticles) {
    int curPos = 0;
    for (int i = 0; i < numOfParticles; i++) {
        Particle temp = Particle((double)1 / numOfParticles, curPos);
        particles.push_back(temp);
        curPos += skipDis;
    }
}

Robot createRobot(vector<double>reads, int len) {
    while (true) {
        cout << "Please enter the initial position of the Robot (must be between 1 and 1000) : ";
        int pos;
        cin >> pos;
        if (pos >= 1 and pos <= len) {
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

int discreteDis(int numberOfParticle, vector<Particle> particles) {
    vector<long long>dis(numberOfParticle);
    for (int i = 0; i < particles.size(); i++) {
        dis[i] = (particles[i].getWeight() * 1000000);
    }

    discrete_distribution<long long> distribution(dis.begin(), dis.end());

    //list to store new particles after resampling
    vector<Particle> newParticles(numberOfParticle);

    //generate new particles with discrete distribution
    std::default_random_engine generator;

    return distribution(generator);
}

void normalize(vector<Particle> &particles, double sum) {
    for (int i = 0; i < particles.size(); i++) {
        particles[i].setWeight(particles[i].getWeight() / sum);
    }
}

double updateAndGetSum(vector<Particle>&particles, vector<double> &positionsRead, double mean, double standardDeviation) {
    double sum = 0;
    for (int i = 0; i < particles.size(); i++) {
        particles[i].setPosition(particles[i].getPosition() + 1);
        double newWeight = weight((positionsRead[i] - positionsRead[particles[i].getPosition()]), mean, standardDeviation);
        particles[i].setWeight(newWeight);
        sum += particles[i].getWeight();
    }
    return sum;
}

void moveInCorridor(Robot &robot, vector<Particle> &particles, vector<double> &positionsRead, double mean, double standardDeviation, int len, int numberOfParticle) {
    bool turn = true;

    while (true) {
        if (turn and robot.getRobotPosition() + 1 >= len) {
            turn = false;
        } else if (!turn and robot.getRobotPosition() - 1 < 1) {
            turn = true;
        }
        if (turn) {
            robot.move(1);
        } else {
            robot.move(-1);
        }

        //move particles 1 step
        int sum = 0;
        for (int i = 0; i < particles.size(); i++) {
            if (turn and particles[i].getPosition() + 1 > len) {
                int index = discreteDis(numberOfParticle, particles);
                particles[i] = particles[index];
            } else if (!turn and particles[i].getPosition() - 1 < 1) {
                int index = discreteDis(numberOfParticle, particles);
                particles[i] = particles[index];
            } else {
                if (turn) {
                    particles[i].setPosition(particles[i].getPosition() + 1);
                } else {
                    particles[i].setPosition(particles[i].getPosition() - 1);
                }
            }
        }

        sum = updateAndGetSum(particles, positionsRead, mean, standardDeviation);
        //normalize the weights after moving
        normalize(particles, sum);

        vector<long long>dis(numberOfParticle);
        for (int i = 0; i < particles.size(); i++) {
            dis[i] = (particles[i].getWeight() * 1000000);
        }

        discrete_distribution<long long> distribution(dis.begin(), dis.end());

        //list to store new particles after resampling
        vector<Particle> newParticles(numberOfParticle);

        //generate new particles with discrete distribution
        std::default_random_engine generator;
        for (int i = 0; i < particles.size(); i++) {
            int index = distribution(generator);
            newParticles[i] = particles[i];
        }

        particles = newParticles;
        set<int> uniquePos;

        for (int i = 0; i < particles.size(); i++) {
            uniquePos.insert(particles[i].getPosition());
        }

    // if all particle are placed in one position, this mean the robot in this position
        if (uniquePos.size() == 1) {
            cout << endl;
            cout << "The robot is in index " << robot.getRobotPosition() << endl;
            return;
        }

        //Update weights after resampling
        sum = updateAndGetSum(particles, positionsRead, mean, standardDeviation);

        //normalize the weights after moving
        normalize(particles, sum);
    }
}

#endif //AI_PARTICLE_FILTER_HELPERS_H


