#pragma once
#include "signal.h"

class UnitImpulse : public Signal {
public:
    UnitImpulse() = default;

    double computeValue(double t) const override;

    std::string getName() const override { 
        return "S10: Unit Impulse"; 
    }
    bool isPeriodic() const override { 
        return false; 
    }
    bool isDiscrete() const override { 
        return true; 
    }

    void generateDiscrete();
};
