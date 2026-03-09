#include "symmetrical_square_wave_signal.h"
#include <cmath>

double SymmetricalSquareWaveSignal::computeValue(double t) const {
    double t1 = params.startTime;
    double t2 = t1 + params.duration;

    if (t < t1 || t > t2) {
        return 0.0;
    }
    double T  = params.period;
    double kw = params.dutyCycle;

    if (T <= 0.0) {
        return 0.0;
    }
    double phase = std::fmod(t - t1, T);

    if (phase < 0.0) {
        phase += T;
    }

    if (phase < kw * T) {
        return params.amplitude;
    }
    return -params.amplitude;
}
