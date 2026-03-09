#pragma once
#include "signal.h"

class SquareWaveSignal : public Signal {
public:
    SquareWaveSignal() = default;
    double computeValue(double t) const override;
    std::string getName() const override { return "S6: Square Wave"; }
    bool isPeriodic() const override { return true; }
    bool isDiscrete() const override { return false; }
};
