#include "unit_step.h"
#include <cmath>

double UnitStep::computeValue(double t) const {
    double t1 = params.startTime;
    double t2 = t1 + params.duration;

    if (t < t1 || t > t2) {
        return 0.0;
    }
    
    double ts = params.stepTime;
    const double eps = 1e-9;

    if (std::abs(t - ts) < eps) {
        return params.amplitude / 2.0;
    }
    if (t > ts) {
        return params.amplitude;
    }
    return 0.0;
}
