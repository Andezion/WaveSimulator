#pragma once
#include "signal.h"

class SingleHalfRectifiedSinusSignal : public Signal {
public:
    SingleHalfRectifiedSinusSignal() = default;
    double computeValue(double t) const override;
    std::string getName() const override { return "S4: Half-Wave Rect. Sine"; }
    bool isPeriodic() const override { return true; }
    bool isDiscrete() const override { return false; }
};
