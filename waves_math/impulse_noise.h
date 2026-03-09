#pragma once
#include "signal.h"

class ImpulseNoise : public Signal {
public:
    ImpulseNoise() = default;
    double computeValue(double t) const override;

    std::string getName() const override { 
        return "S11: Impulse Noise"; 
    }
    bool isPeriodic() const override { 
        return false; 
    }
    bool isDiscrete() const override { 
        return true; 
    }
};
