#pragma once
#include "signal.h"

class UniformDistributedNoise : public Signal {
public:
    UniformDistributedNoise() = default;
    double computeValue(double t) const override;
    std::string getName() const override { return "S1: Uniform Noise"; }
    bool isPeriodic() const override { return false; }
    bool isDiscrete() const override { return false; }
};
