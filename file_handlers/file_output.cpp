#include "file_io.h"
#include <cstdio>
#include <cstdint>

bool saveSignal(const Signal& sig, const std::string& path) {
    FILE* f = fopen(path.c_str(), "wb"); // Открываем файл для записи в бинарном режиме
    if (!f) {
        return false;
    }

    double startTime = sig.params.startTime; // Время начала сигнала, по которому будут вычисляться времена отсчетов сигнала
    double samplingFreq = sig.params.samplingFreq; // Частота дискретизации сигнала, по которой будут вычисляться времена отсчетов сигнала
    int32_t isComplex = 0; // Флаг, указывающий, является ли сигнал комплексным - в данном случае мы сохраняем только действительные сигналы, поэтому устанавливаем его в 0
    int32_t numSamples = static_cast<int32_t>(sig.samples.size()); // Количество отсчетов сигнала, которое будет сохранено в файл - должно быть неотрицательным и не слишком большим, иначе сохранение считается ошибкой

    fwrite(&startTime,sizeof(double),  1, f);
    fwrite(&samplingFreq,sizeof(double),  1, f);
    fwrite(&isComplex,sizeof(int32_t), 1, f);
    fwrite(&numSamples,sizeof(int32_t), 1, f);

    if (numSamples > 0) {
        fwrite(sig.samples.data(), sizeof(double), static_cast<size_t>(numSamples), f);
    }

    fclose(f);
    return true;
}
