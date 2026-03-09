#include "square_wave_signal.h"
#include <cmath>

double SquareWaveSignal::computeValue(double t) const {
    double t1 = params.startTime;
    double t2 = t1 + params.duration;
    if (t < t1 || t > t2) return 0.0;
    double T  = params.period;
    double kw = params.dutyCycle;
    if (T <= 0.0) return 0.0;
    // Phase within current period
    double phase = std::fmod(t - t1, T);
    if (phase < 0.0) phase += T;
    // A in [0, kw*T), 0 in [kw*T, T)
    if (phase < kw * T) return params.amplitude;
    return 0.0;
}
