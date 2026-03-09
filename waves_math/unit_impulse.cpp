#include "unit_impulse.h"
#include <cmath>

double UnitImpulse::computeValue(double t) const {
    int n = static_cast<int>(std::round(t * params.samplingFreq));
    int ns = params.sampleStep; 

    if (n == ns) {
        return params.amplitude;
    }
    return 0.0;
}

void UnitImpulse::generateDiscrete() {
    samples.clear();
    times.clear();

    int numSamples = static_cast<int>(params.duration * params.samplingFreq);

    if (numSamples <= 0) {
        numSamples = 1;
    }

    for (int n = params.sampleStart; n < params.sampleStart + numSamples; ++n) {
        double t = static_cast<double>(n) / params.samplingFreq;

        times.push_back(t);
        samples.push_back(computeValue(t));
    }
}
