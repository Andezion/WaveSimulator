#include "gaussian_noise.h"
#include <cstdlib>
#include <cmath>

static double gaussianRand() {
    static bool hasSpare = false;
    static double spare;

    if (hasSpare) {
        hasSpare = false;
        return spare;
    }

    double u, v, s;
    do {
        u = (static_cast<double>(rand()) / RAND_MAX) * 2.0 - 1.0;
        v = (static_cast<double>(rand()) / RAND_MAX) * 2.0 - 1.0;
        s = u * u + v * v;
    } while (s >= 1.0 || s == 0.0);

    double mul = std::sqrt(-2.0 * std::log(s) / s);

    spare = v * mul;
    hasSpare = true;

    return u * mul;
}

double GaussianNoise::computeValue(double /*t*/) const {
    return params.amplitude * gaussianRand();
}
