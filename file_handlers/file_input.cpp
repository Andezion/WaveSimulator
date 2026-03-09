#include "file_io.h"
#include "../waves_math/signal.h"
#include <cstdio>
#include <cstdint>
#include <memory>

std::unique_ptr<Signal> loadSignal(const std::string& path) {
    FILE* f = fopen(path.c_str(), "rb");
    if (!f) {
        return nullptr;
    }

    double  startTime    = 0.0;
    double  samplingFreq = 1.0;
    int32_t isComplex    = 0;
    int32_t numSamples   = 0;

    if (fread(&startTime,    sizeof(double),  1, f) != 1) { 
        fclose(f); return nullptr; 
    }
    if (fread(&samplingFreq, sizeof(double),  1, f) != 1) { 
        fclose(f); return nullptr; 
    }
    if (fread(&isComplex,    sizeof(int32_t), 1, f) != 1) { 
        fclose(f); return nullptr; 
    }
    if (fread(&numSamples,   sizeof(int32_t), 1, f) != 1) { 
        fclose(f); return nullptr; 
    }

    if (numSamples < 0 || numSamples > 100000000) { 
        fclose(f); return nullptr; 
    }

    auto sig = std::make_unique<GenericSignal>();
    sig->params.startTime    = startTime;
    sig->params.samplingFreq = samplingFreq;

    sig->samples.resize(static_cast<size_t>(numSamples));
    if (numSamples > 0) {
        size_t read = fread(sig->samples.data(), sizeof(double), static_cast<size_t>(numSamples), f);
        sig->samples.resize(read);
    }

    sig->times.resize(sig->samples.size());
    for (size_t i = 0; i < sig->samples.size(); ++i) {
        sig->times[i] = startTime + static_cast<double>(i) / samplingFreq;
    }

    sig->params.duration = sig->samples.empty() ? 0.0
        : sig->times.back() - sig->times.front();

    fclose(f);
    return sig;
}
