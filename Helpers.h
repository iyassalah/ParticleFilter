
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
    return  sum / 1000;
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

void moveInCorridor(Robot &robot, vector<Particle> &particles, vector<double> &positionsRead, double mean, double standardDeviation) {
    bool turn = true;

    while (true) {
//        cout << robot.getRobotPosition() << endl;
        /*for (int i = 0; i < particles.size(); i++) {
            cout << particles[i].getWeight() << endl;
        }*/
        if (turn and robot.getRobotPosition() + 1 > 1000) {
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
            if (turn and particles[i].getPosition() + 1 > 1000) {
                Particle temp = new Particle();
                temp.setPosition(generateRandom(999));
                temp.setWeight(weight((positionsRead[i] - positionsRead[temp.getPosition()]), mean, standardDeviation));
                sum += temp.getWeight();
                particles[i] = temp;
            } else if (!turn and particles[i].getPosition() - 1 < 1) {
                Particle temp = new Particle();
                temp.setPosition(generateRandom(999));
                temp.setWeight(weight((positionsRead[i] - positionsRead[temp.getPosition()]), mean, standardDeviation));
                sum += temp.getWeight();
                particles[i] = temp;
            } else {
                if (turn) {
                    particles[i].setPosition(particles[i].getPosition() + 1);
                } else {
                    particles[i].setPosition(particles[i].getPosition() - 1);
                }
            }
            double newWeight = weight((positionsRead[i] - positionsRead[particles[i].getPosition()]), mean, standardDeviation);
            particles[i].setWeight(newWeight);
            sum += particles[i].getWeight();
        }

        //normalize the weights after moving
        for (int i = 0; i < particles.size(); i++) {
            particles[i].setWeight(particles[i].getWeight() / sum);
        }

        vector<long long>dis(50);
        for (int i = 0; i < particles.size(); i++) {
            dis[i] = (particles[i].getWeight() * 1000000);
        }

        discrete_distribution<long long> distribution(dis.begin(), dis.end());

        //list to store new particles after resampling
        vector<Particle> newParticles(50);

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
        cout << uniquePos.size() << endl;

        if (uniquePos.size() == 1) {
            cout << "The robot is in index " << *uniquePos.begin() << endl;
            return;
        }

        //Update weights after resampling
        sum = 0;
        for (int i = 0; i < particles.size(); i++) {
            particles[i].setPosition(particles[i].getPosition() + 1);
            double newWeight = weight((positionsRead[i] - positionsRead[particles[i].getPosition()]), mean, standardDeviation);
            particles[i].setWeight(newWeight);
            sum += particles[i].getWeight();
        }
        //normalize the weights after moving
        for (int i = 0; i < particles.size(); i++) {
            particles[i].setWeight(particles[i].getWeight() / sum);
        }
    }
}

#endif //AI_PARTICLE_FILTER_HELPERS_H


