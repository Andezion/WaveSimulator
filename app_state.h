#pragma once
#include "waves_math/signal.h"
#include <memory>
#include <string>
#include <array>

// Структура для хранения состояния приложения, включая выбранный тип сигнала, параметры, текущий сигнал и так далее
enum class SignalType { None = 0, S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11 };
// Типы операций для обработки сигналов
enum class Operation  { Add = 0, Sub, Mul, Div };

// Структура для текстового ввода (например, для параметров сигналов и всякой хуйни)
struct TextInput {
    char buf[64] = {};
    bool active  = false;
};

// Структура для хранения всех параметров сигнала, которые могут быть отредактированы пользователем
enum ParamIndex {
    PI_AMPLITUDE = 0,
    PI_START_TIME = 1,
    PI_DURATION = 2,
    PI_PERIOD = 3,
    PI_DUTY_CYCLE = 4,
    PI_SAMPLING_FREQ= 5,
    PI_STEP_TIME = 6,
    PI_SAMPLE_START = 7,
    PI_SAMPLE_STEP = 8,
    PI_PROBABILITY = 9,
    PI_COUNT = 10
};

// Основная структура для хранения состояния приложения, которая будет использоваться во всех обработчиках и функциях отрисовки
struct AppState {
    // Дефолтные значения параметров для генерации сигналов
    SignalType selectedType = SignalType::S3;
    // Дефолтный параметр для операций над сигналами
    Operation selectedOp = Operation::Add;

    // Наши параметры сигнала
    SignalParams params;

    // Текстовые поля для редактирования параметров сигнала
    std::array<TextInput, PI_COUNT> paramInputs;
    int focusedInput = -1; // Индекс параметра который меняем

    std::unique_ptr<Signal> currentSignal; // Сгенерированный или загруженный сигнал
    std::unique_ptr<Signal> opSignal1; // Первый сигнал для операций над сигналами
    std::unique_ptr<Signal> opSignal2; // Второй сигнал для операций над сигналами
    std::unique_ptr<Signal> resultSignal; // Результат операции над сигналами (если применимо)

    int histBins = 10; // Количество бинов для гистограммы
    float plotScrollX = 0.0f; // Прокрутка графика по горизонтали
    float plotZoom = 1.0f; // Масштаб графика (1.0 = 100%)

    std::string statusMsg; // Сообщение для отображения в статусной строке (например, ошибки при загрузке/сохранении)
    std::string opFile1Path; // Путь к первому сигналу для операций над сигналами
    std::string opFile2Path; // Путь ко второму сигналу для операций над сигналами

    double statMean    = 0.0;
    double statAbsMean = 0.0;
    double statRms     = 0.0;
    double statVar     = 0.0;
    double statPower   = 0.0;

    bool   statsValid  = false;

    bool   showSaveDialog  = false;
    bool   showLoadDialog  = false;
    bool   showOp1Dialog   = false;
    bool   showOp2Dialog   = false;
    
    TextInput fileNameInput;

    void init();
    void syncParamsToInputs();
    void syncInputsToParams();
    void updateStats();
    void generateSignal();
};
