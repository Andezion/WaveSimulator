#include "double_half_rectified_sinus_signal.h"
#include <cmath>

double DoubleHalfRectifiedSinusSignal::computeValue(double t) const {
    double t1 = params.startTime;
    double t2 = t1 + params.duration;
    if (t < t1 || t > t2) return 0.0;
    double T = params.period;
    if (T <= 0.0) return 0.0;
    // x(t) = A * |sin(2*pi/T * (t - t1))|
    return params.amplitude * std::abs(std::sin(2.0 * M_PI / T * (t - t1)));
}
