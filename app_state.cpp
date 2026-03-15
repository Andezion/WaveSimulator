#include "app_state.h"
#include "waves_math/uniform_distributed_noise.h"
#include "waves_math/gaussian_noise.h"
#include "waves_math/sinus_signal.h"
#include "waves_math/single_half_rectified_sinus_signal.h"
#include "waves_math/double_half_rectified_sinus_signal.h"
#include "waves_math/square_wave_signal.h"
#include "waves_math/symmetrical_square_wave_signal.h"
#include "waves_math/triangular_wave_signal.h"
#include "waves_math/unit_step.h"
#include "waves_math/unit_impulse.h"
#include "waves_math/impulse_noise.h"
#include <cstdio>
#include <cstdlib>

// Выбор генератора сигнала в зависимости от выбранного типа
static std::unique_ptr<Signal> makeSignal(SignalType type) {
    switch (type) {
        case SignalType::S1:  return std::make_unique<UniformDistributedNoise>();
        case SignalType::S2:  return std::make_unique<GaussianNoise>();
        case SignalType::S3:  return std::make_unique<SinusSignal>();
        case SignalType::S4:  return std::make_unique<SingleHalfRectifiedSinusSignal>();
        case SignalType::S5:  return std::make_unique<DoubleHalfRectifiedSinusSignal>();
        case SignalType::S6:  return std::make_unique<SquareWaveSignal>();
        case SignalType::S7:  return std::make_unique<SymmetricalSquareWaveSignal>();
        case SignalType::S8:  return std::make_unique<TriangularWaveSignal>();
        case SignalType::S9:  return std::make_unique<UnitStep>();
        case SignalType::S10: return std::make_unique<UnitImpulse>();
        case SignalType::S11: return std::make_unique<ImpulseNoise>();
        default:              return nullptr;
    }
}

// Инициализация состояния приложения с дефолтными параметрами
void AppState::init() {
    params = SignalParams{};

    selectedType = SignalType::S1;
    selectedOp = Operation::Add;

    histBins = 10;
    plotScrollX = 0.0f;
    plotZoom = 1.0f;

    statsValid = false;

    statusMsg.clear();

    opFile1Path.clear();
    opFile2Path.clear();

    focusedInput = -1;

    syncParamsToInputs();
}

// Синхронизация параметров сигнала в структуре AppState с текстовыми полями для редактирования
void AppState::syncParamsToInputs() {
    snprintf(paramInputs[PI_AMPLITUDE].buf,sizeof(TextInput::buf), "%.4g", params.amplitude);
    snprintf(paramInputs[PI_START_TIME].buf,sizeof(TextInput::buf), "%.4g", params.startTime);
    snprintf(paramInputs[PI_DURATION].buf,sizeof(TextInput::buf), "%.4g", params.duration);
    snprintf(paramInputs[PI_PERIOD].buf,sizeof(TextInput::buf), "%.4g", params.period);
    snprintf(paramInputs[PI_DUTY_CYCLE].buf,sizeof(TextInput::buf), "%.4g", params.dutyCycle);
    snprintf(paramInputs[PI_SAMPLING_FREQ].buf,sizeof(TextInput::buf), "%.4g", params.samplingFreq);
    snprintf(paramInputs[PI_STEP_TIME].buf,sizeof(TextInput::buf), "%.4g", params.stepTime);
    snprintf(paramInputs[PI_SAMPLE_START].buf,sizeof(TextInput::buf), "%d", params.sampleStart);
    snprintf(paramInputs[PI_SAMPLE_STEP].buf,sizeof(TextInput::buf), "%d", params.sampleStep);
    snprintf(paramInputs[PI_PROBABILITY].buf,sizeof(TextInput::buf), "%.4g", params.probability);
}

// Парсинг текстовых полей для редактирования параметров сигнала и сохранение их в структуре AppState
void AppState::syncInputsToParams() {
    params.amplitude = atof(paramInputs[PI_AMPLITUDE].buf);
    params.startTime = atof(paramInputs[PI_START_TIME].buf);
    params.duration = atof(paramInputs[PI_DURATION].buf);
    params.period  = atof(paramInputs[PI_PERIOD].buf);
    params.dutyCycle = atof(paramInputs[PI_DUTY_CYCLE].buf);
    params.samplingFreq = atof(paramInputs[PI_SAMPLING_FREQ].buf);
    params.stepTime = atof(paramInputs[PI_STEP_TIME].buf);
    params.sampleStart = atoi(paramInputs[PI_SAMPLE_START].buf);
    params.sampleStep = atoi(paramInputs[PI_SAMPLE_STEP].buf);
    params.probability = atof(paramInputs[PI_PROBABILITY].buf);

    if (params.samplingFreq <= 0.0) {
        params.samplingFreq = 1.0;
    }
    if (params.duration <= 0.0) {
        params.duration = 0.01;
    }
    if (params.period <= 0.0) {
        params.period = 0.01;
    }
    if (params.dutyCycle < 0.0) {
        params.dutyCycle = 0.0;
    }
    if (params.dutyCycle > 1.0) {
        params.dutyCycle = 1.0;
    }
    if (params.probability < 0.0) {
        params.probability = 0.0;
    }
    if (params.probability > 1.0) {
        params.probability = 1.0;
    }
}

// Вычесление новых статистик для выбранного сигнала
void AppState::updateStats() {
    statsValid = false;

    if (!currentSignal || currentSignal->samples.empty()) {
        return;
    }

    statMean = currentSignal->mean();
    statAbsMean = currentSignal->absMean();
    statRms = currentSignal->rms();
    statVar = currentSignal->variance();
    statPower = currentSignal->power();
    statsValid = true;
}

// Генерация и отрисовка сигнала который выбрали
void AppState::generateSignal() {
    syncInputsToParams(); // берём параметры из полей 
    auto sig = makeSignal(selectedType); // создаём сигнал в зависимости от выбранного типа

    if (!sig) { 
        statusMsg = "Unknown signal type"; 
        return; 
    }

    sig->params = params; // устанавливаем параметры сигнала
    sig->generate(); // генерируем сигнал

    currentSignal = std::move(sig); // поскольку работаем с unique_ptr, то просто перемещаем новый сигнал в currentSignal
    plotScrollX = 0.0f;
    
    updateStats(); // обновляем статистику для нового сигнала
    statusMsg = "Signal generated: " + currentSignal->getName();
}
