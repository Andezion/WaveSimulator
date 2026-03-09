#include "uniform_distributed_noise.h"
#include <cstdlib>

double UniformDistributedNoise::computeValue(double /*t*/) const {
    // Returns random value in [-A, A]
    double r = static_cast<double>(rand()) / RAND_MAX;
    return params.amplitude * (2.0 * r - 1.0);
}
