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
    // Phase within current period (relative to t1)
    double phase = std::fmod(t - t1, T);
    if (phase < 0.0) phase += T;
    double risingEnd = kw * T;
    if (risingEnd <= 0.0) risingEnd = 1e-9;
    double fallingLen = T - risingEnd;
    if (phase < risingEnd) {
        // Rising: A/(kw*T) * phase
        return A / risingEnd * phase;
    } else {
        // Falling: -A/(T*(1-kw)) * (phase - kw*T) + A/(1-kw)
        // Simplified: starts at A when phase==kw*T, ends at -A... actually:
        // The formula from spec: -A/(T*(1-kw))*(t-kT-t1) + A/(1-kw)
        // where t-kT-t1 is the phase within the period.
        // At phase=kw*T: -A/(T*(1-kw))*kw*T + A/(1-kw) = -A*kw/(1-kw) + A/(1-kw) = A*(1-kw)/(1-kw) = A. Good.
        // At phase=T: -A/(T*(1-kw))*T + A/(1-kw) = -A/(1-kw) + A/(1-kw) = 0. Good.
        if (fallingLen <= 0.0) return A;
        return -A / (T * (1.0 - kw)) * phase + A / (1.0 - kw);
    }
}
