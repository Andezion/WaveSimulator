#pragma once
#include "signal.h"

class SymmetricalSquareWaveSignal : public Signal {
public:
    SymmetricalSquareWaveSignal() = default;
    double computeValue(double t) const override;

    std::string getName() const override { 
        return "S7: Sym. Square Wave"; 
    }
    bool isPeriodic() const override { 
        return true; 
    }
    bool isDiscrete() const override { 
        return false; 
    }
};
