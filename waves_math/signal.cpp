#include "waves_math/signal.h"

void Signal::generate() {
    samples.clear();
    times.clear();

    if (isDiscrete()) {
        int numSamples = static_cast<int>(params.duration * params.samplingFreq);
        if (numSamples <= 0) { 
            numSamples = 1;
        }
        for (int n = params.sampleStart; n < params.sampleStart + numSamples; ++n) {
            double t = static_cast<double>(n) / params.samplingFreq;
            times.push_back(t);
            samples.push_back(computeValue(t));
        }
    } else {
        double t2   = params.startTime + params.duration;
        double step = 1.0 / params.samplingFreq;
        for (double t = params.startTime; t <= t2 + step * 0.5; t += step) {
            times.push_back(t);
            samples.push_back(computeValue(t));
        }
    }
}

std::pair<int,int> Signal::effectiveRange() const {
    if (samples.empty()) return {0, 0};
    if (!isPeriodic() || params.period <= 0.0) {
        return {0, static_cast<int>(samples.size()) - 1};
    }
    int total = static_cast<int>(samples.size());
    double step = 1.0 / params.samplingFreq;
    int samplesPerPeriod = static_cast<int>(std::round(params.period / step));
    if (samplesPerPeriod < 1) samplesPerPeriod = 1;
    int numPeriods = total / samplesPerPeriod;
    if (numPeriods < 1) numPeriods = 1;
    int endIdx = numPeriods * samplesPerPeriod;
    if (endIdx > total) endIdx = total;
    return {0, endIdx - 1};
}

double Signal::mean() const {
    if (samples.empty()) return 0.0;
    auto [s, e] = effectiveRange();
    int count = e - s + 1;
    if (count <= 0) return 0.0;
    double sum = 0.0;
    for (int i = s; i <= e; ++i) sum += samples[i];
    return sum / count;
}

double Signal::absMean() const {
    if (samples.empty()) return 0.0;
    auto [s, e] = effectiveRange();
    int count = e - s + 1;
    if (count <= 0) return 0.0;
    double sum = 0.0;
    for (int i = s; i <= e; ++i) sum += std::abs(samples[i]);
    return sum / count;
}

double Signal::rms() const {
    if (samples.empty()) return 0.0;
    auto [s, e] = effectiveRange();
    int count = e - s + 1;
    if (count <= 0) return 0.0;
    double sum = 0.0;
    for (int i = s; i <= e; ++i) sum += samples[i] * samples[i];
    return std::sqrt(sum / count);
}

double Signal::variance() const {
    if (samples.empty()) return 0.0;
    double m = mean();
    auto [s, e] = effectiveRange();
    int count = e - s + 1;
    if (count <= 0) return 0.0;
    double sum = 0.0;
    for (int i = s; i <= e; ++i) {
        double diff = samples[i] - m;
        sum += diff * diff;
    }
    return sum / count;
}

double Signal::power() const {
    double r = rms();
    return r * r;
}
