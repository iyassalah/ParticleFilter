
#ifndef AI_PARTICLE_FILTER_HELPERS_H
#define AI_PARTICLE_FILTER_HELPERS_H

#include <cmath>
#include <iostream>
#include <vector>
#include <random>
#include <functional>
#include <random>
#include <set>
#include <map>
#include <unordered_map>

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
    return ((1 / (q * sqrt(2 * PI))) * exp((-1 * pow((x - u), 2)) / (2 * pow(q, 2))));
}

unsigned long long generateRandom(unsigned long long mod) {
    return rand() % mod;
}

void calcNewWeights(vector<Particle> &particles, vector<double>&posRead, Robot &robot, double mean, double SD) {
    double sum = 0;
    vector<Particle> newParicles;

    ///resampling particles
    for (int i = 0; i < particles.size(); i++) {
        Particle temp = new Particle();
        temp.setPosition(particles[i].getPosition());
        temp.setWeight(weight((posRead[i] - posRead[robot.getRobotPosition()]), mean, SD));
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

void getInitialDistribution(vector<Particle> &particles, int skipDis, int numOfParticles) {
    int curPos = 0;
    for (int i = 0; i < numOfParticles; i++) {
        Particle temp = Particle((double) 1 / numOfParticles, curPos);
        particles.push_back(temp);
        curPos += skipDis;
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

double updateAndGetSum(vector<Particle>&particles, vector<double> &positionsRead, double mean, double standardDeviation, Robot &robot) {
    double sum = 0;
    for (int i = 0; i < particles.size(); i++) {
        double newWeight = weight(( positionsRead[particles[i].getPosition()]) - positionsRead[robot.getRobotPosition()], mean, standardDeviation);
        particles[i].setWeight(newWeight);
        sum += particles[i].getWeight();
    }
    return sum;
}

/// 1. move particles with random speed for each.
/// 2. do not change the particle when it's going out of range, but move it such as robot when it's.
///

void moveInCorridor(Robot &robot, vector<Particle> &particles, vector<double> &positionsRead, double &mean, double &standardDeviation, int len, int numberOfParticle) {
    bool turn = true;

    while (true) {
        /*for (auto element : particles) {
            cout << element.getPosition() << " " << (element.isDir() ? "true" : "false") << endl;
        }
        cout << endl << endl;*/
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

        for (int i = 0; i < particles.size(); i++) {
            int step = rand() % 5;
            if (particles[i].isDir() and particles[i].getPosition() + step > len) {
                particles[i].setDir(false);
            } else if (!particles[i].isDir() and particles[i].getPosition() - step < 1) {
                particles[i].setDir(true);
            }

            if (particles[i].isDir()) {
                particles[i].setPosition(particles[i].getPosition() + step);
            } else {
                particles[i].setPosition(particles[i].getPosition() - step);
            }
        }

        double sum = updateAndGetSum(particles, positionsRead, mean, standardDeviation, robot);
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
            newParticles[i] = particles[index];
        }

        particles = newParticles;
//        set<int> uniquePos;
        map<int, int> freq;
        for (int i = 0; i < particles.size(); i++) {
//            uniquePos.insert(particles[i].getPosition());
            freq[particles[i].getPosition()]++;
        }
        pair<int, int> bestPos = {-1, -1};
        int cur = 0;
        for (auto x : freq) {
            if (x.second > bestPos.second) {
                bestPos.second = x.second;
                bestPos.first = x.first;
            }
            cur++;
        }

        // if all particle are placed in one position, this mean the robot in this position
        if (freq.size() == 10) {
            cout << endl;
            cout << "The robot was found when the robot reach position " << bestPos.first << endl;
            /*for (auto x : freq) {
                cout << x.first << endl;
            }
            cout << bestPos.first << endl;*/
            return;
        }

        //Update weights after resampling
        sum = updateAndGetSum(particles, positionsRead, mean, standardDeviation, robot);

        //normalize the weights after moving
        normalize(particles, sum);
    }
}

#endif //AI_PARTICLE_FILTER_HELPERS_H


