#pragma once
#include "signal.h"

class DoubleHalfRectifiedSinusSignal : public Signal {
public:
    DoubleHalfRectifiedSinusSignal() = default;
    double computeValue(double t) const override;

    std::string getName() const override { 
        return "S5: Full-Wave Rect. Sine"; 
    }
    bool isPeriodic() const override { 
        return true; 
    }
    bool isDiscrete() const override { 
        return false; 
    }
};
