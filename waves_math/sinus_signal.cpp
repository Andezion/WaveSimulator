#include "sinus_signal.h"
#include <cmath>

double SinusSignal::computeValue(double t) const {
    double t1 = params.startTime;
    double t2 = t1 + params.duration;
    if (t < t1 || t > t2) return 0.0;
    double T = params.period;
    if (T <= 0.0) return 0.0;
    return params.amplitude * std::sin(2.0 * M_PI / T * (t - t1));
}
