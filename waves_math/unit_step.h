#pragma once
#include "signal.h"

class UnitStep : public Signal {
public:
    UnitStep() = default;
    double computeValue(double t) const override;

    std::string getName() const override { 
        return "S9: Unit Step"; 
    }
    bool isPeriodic() const override { 
        return false; 
    }
    bool isDiscrete() const override { 
        return false; 
    }
};
