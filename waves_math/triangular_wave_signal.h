#pragma once
#include "signal.h"

class TriangularWaveSignal : public Signal {
public:
    TriangularWaveSignal() = default;
    double computeValue(double t) const override;

    std::string getName() const override { 
        return "S8: Triangular Wave"; 
    }
    bool isPeriodic() const override { 
        return true; 
    }
    bool isDiscrete() const override { 
        return false; 
    }
};
