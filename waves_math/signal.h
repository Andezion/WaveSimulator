#pragma once
#include <vector>
#include <string>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct SignalParams {
    double amplitude    = 1.0;
    double startTime    = 0.0;
    double duration     = 10.0;
    double period       = 1.0;
    double dutyCycle    = 0.5;
    double samplingFreq = 100.0;
    double stepTime     = 5.0;
    int    sampleStart  = 0;
    int    sampleStep   = 0;
    double probability  = 0.5;
};

class Signal {
public:
    SignalParams params;
    std::vector<double> samples;
    std::vector<double> times;

    Signal() = default;
    virtual ~Signal() = default;

    virtual double computeValue(double t) const = 0;
    virtual std::string getName() const = 0;
    virtual bool isPeriodic() const { return false; }
    virtual bool isDiscrete() const { return false; }

    void generate();
    double mean() const;
    double absMean() const;
    double rms() const;
    double variance() const;
    double power() const;

protected:
    std::pair<int,int> effectiveRange() const;
};

// Concrete signal that holds pre-loaded samples (used for file I/O and operations)
class GenericSignal : public Signal {
public:
    std::string name = "Loaded Signal";
    bool discrete    = false;

    GenericSignal() = default;
    double computeValue(double /*t*/) const override { return 0.0; }
    std::string getName() const override { return name; }
    bool isPeriodic() const override { return false; }
    bool isDiscrete() const override { return discrete; }
};
