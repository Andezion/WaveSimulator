#pragma once
#include "signal.h"

class GaussianNoise : public Signal {
public:
    GaussianNoise() = default;
    double computeValue(double t) const override;

    std::string getName() const override { 
        return "S2: Gaussian Noise"; 
    }

    bool isPeriodic() const override { 
        return false; 
    }
    bool isDiscrete() const override { 
        return false; 
    }
};
