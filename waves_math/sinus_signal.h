#pragma once
#include "signal.h"

class SinusSignal : public Signal {
public:
    SinusSignal() = default;
    double computeValue(double t) const override;

    std::string getName() const override { 
        return "S3: Sinusoidal"; 
    }
    bool isPeriodic() const override { 
        return true; 
    }
    bool isDiscrete() const override { 
        return false; 
    }
};
