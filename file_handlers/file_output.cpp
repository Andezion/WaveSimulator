#include "file_io.h"
#include <cstdio>
#include <cstdint>

bool saveSignal(const Signal& sig, const std::string& path) {
    FILE* f = fopen(path.c_str(), "wb");
    if (!f) return false;

    double startTime    = sig.params.startTime;
    double samplingFreq = sig.params.samplingFreq;
    int32_t isComplex   = 0;
    int32_t numSamples  = static_cast<int32_t>(sig.samples.size());

    fwrite(&startTime,    sizeof(double),  1, f);
    fwrite(&samplingFreq, sizeof(double),  1, f);
    fwrite(&isComplex,    sizeof(int32_t), 1, f);
    fwrite(&numSamples,   sizeof(int32_t), 1, f);

    if (numSamples > 0) {
        fwrite(sig.samples.data(), sizeof(double), static_cast<size_t>(numSamples), f);
    }

    fclose(f);
    return true;
}
