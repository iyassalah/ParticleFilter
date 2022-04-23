
#ifndef AI_PARTICLE_FILTER_PARTICLE_H
#define AI_PARTICLE_FILTER_PARTICLE_H


class Particle {
public:

    Particle(long double weight, int position) {
        setWeight(weight);
        setPosition(position);
    }

    Particle() {
    }

    Particle(Particle *pParticle) {

    }

    void setWeight(double W) {
        this->weight = W;
    }

    void setPosition(int pos) {
        this->position = pos;
    }

    double getWeight() {
        return this->weight;
    }

    int getPosition() {
        return this->position;
    }

    /*Particle &operator=(Particle &P) {
        setWeight(P.getWeight());
        setPosition(P.getPosition());

        return *this;
    }*/ /// overrider for = operator with Paricles

private:
    double weight;
    int position;
};


#endif //AI_PARTICLE_FILTER_PARTICLE_H
