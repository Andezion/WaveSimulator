#include "triangular_wave_signal.h"
#include <cmath>

double TriangularWaveSignal::computeValue(double t) const {
    double t1 = params.startTime;
    double t2 = t1 + params.duration;
    if (t < t1 || t > t2) return 0.0;
    double T  = params.period;
    double kw = params.dutyCycle;
    if (T <= 0.0) return 0.0;
    double A = params.amplitude;
    double phase = std::fmod(t - t1, T);
    if (phase < 0.0) phase += T;
    double risingEnd = kw * T;
    if (risingEnd <= 0.0) risingEnd = 1e-9;
    double fallingLen = T - risingEnd;
    if (phase < risingEnd) {
        return A / risingEnd * phase;
    } else {
        if (fallingLen <= 0.0) return A;
        return -A / (T * (1.0 - kw)) * phase + A / (1.0 - kw);
    }
}
