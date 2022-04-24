
#ifndef AI_PARTICLE_FILTER_SENSOR_H
#define AI_PARTICLE_FILTER_SENSOR_H


class Sensor {
public:
    Sensor() {

    }

    Sensor(std::vector<double> posReads) {
        this->reads = posReads;
    }
private:
    std::vector<double> reads;
};


#endif //AI_PARTICLE_FILTER_SENSOR_H
