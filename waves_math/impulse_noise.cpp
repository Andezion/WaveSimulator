#include "impulse_noise.h"
#include <cstdlib>

double ImpulseNoise::computeValue(double /*t*/) const {
    double r = static_cast<double>(rand()) / RAND_MAX;

    if (r < params.probability) { 
        return params.amplitude;
    }
    return 0.0;
}
