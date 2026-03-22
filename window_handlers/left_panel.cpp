#include "left_panel.h"
#include "ui_font.h"
#include "ui_widgets.h"
#include <cstdio>
#include <cstring>
#include <string>

// Вспомогательная функция для обработки выбранной операции над сигналами
void processSignalOperation(AppState& state);

// Рисуем левую панель с элементами управления и информацией о сигнале
void drawLeftPanel(AppState& state) {
    int screenH = GetScreenHeight(); // Получаем высоту экрана для адаптивного размещения элементов

    // Рисуем фон панели и разделительную линию
    Rectangle panel = {0, 0, static_cast<float>(PANEL_W), static_cast<float>(screenH)};
    DrawRectangleRec(panel, COL_PANEL);
    DrawLine(PANEL_W, 0, PANEL_W, screenH, COL_BTN_BORDER);

    float x = 6.0f; // Левый отступ для элементов на панели
    float y = 6.0f; // Верхний отступ для элементов на панели
    float fw = PANEL_W - 12.0f; // Доступная ширина для элементов на панели с учетом отступов
    float bh = 22.0f; // Стандартная высота кнопок и полей ввода
    float bw4 = (fw - 3.0f * 4.0f) / 4.0f; // Ширина для кнопок, если их 4 в ряд

    drawText("Signal Type:", static_cast<int>(x), static_cast<int>(y), FONT_SZ, DARKGRAY);
    y += 18.0f; // Вертикальный отступ после заголовка

    // Рисуем кнопки для выбора типа сигнала. При нажатии на кнопку обновляем выбранный тип в состоянии приложения
    const char* sigLabels[11] = {"S1","S2","S3","S4","S5","S6","S7","S8","S9","S10","S11"};
    // Проходим по всем 11 типам сигналов и рисуем для каждого кнопку. Если тип сигнала соответствует выбранному, подсвечиваем кнопку
    for (int i = 0; i < 11; ++i) {
        int col = i % 4, row = i / 4; // Вычисляем позицию кнопки в зависимости от ее индекса
        // Вычисляем прямоугольник для кнопки с учетом отступов и размера
        Rectangle br = { x + col*(bw4 + 4), y + row*(bh + 4), bw4, bh };
        SignalType st = static_cast<SignalType>(i + 1); // Преобразуем индекс в тип сигнала (начинается с 1)

        drawButton(br, sigLabels[i], state.selectedType == st);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mp = GetMousePosition();
            if (CheckCollisionPointRec(mp, br)) {
                state.selectedType = st;
                state.focusedInput = -1;
            }
        }
    }

    y += 3.0f * (bh + 4.0f) + 8.0f; // Вертикальный отступ после блока выбора типа сигнала

    float fh = 22.0f; // Высота полей ввода для параметров сигнала
    float fy_step = 40.0f; // Вертикальный шаг между полями ввода параметров сигнала

    // Лямбда-функция для рисования поля ввода с меткой. Принимает индекс поля, текст метки и флаг отображения. Если флаг false, поле не рисуется
    auto field = [&](int idx, const char* label, bool show) {
        if (!show) {
            return;
        }
        drawTextInput(state, idx, {x, y, fw, fh}, label);
        y += fy_step;
    };

    // В зависимости от выбранного типа сигнала отображаем соответствующие поля ввода для параметров сигнала
    SignalType t = state.selectedType;
    bool needPeriod = (t == SignalType::S3 || t == SignalType::S4 || t == SignalType::S5 ||
                       t == SignalType::S6 || t == SignalType::S7 || t == SignalType::S8);
    bool needKw = (t == SignalType::S6 || t == SignalType::S7 || t == SignalType::S8);
    bool needTs = (t == SignalType::S9);
    bool needSamples = (t == SignalType::S10);
    bool needFreq = (t != SignalType::S10);
    bool needProb = (t == SignalType::S11);

    field(PI_AMPLITUDE,"Amplitude (A):",true);
    field(PI_START_TIME,"Start Time (t1):",true);
    field(PI_DURATION,"Duration (d):",true);
    field(PI_PERIOD,"Period (T):",needPeriod);
    field(PI_DUTY_CYCLE,"Duty Cycle (kw):",needKw);
    field(PI_SAMPLING_FREQ,"Sampling Freq (f):",needFreq);
    field(PI_STEP_TIME,"Step Time (ts):",needTs);
    field(PI_SAMPLE_START,"Sample Start (n1):",needSamples);
    field(PI_SAMPLE_STEP,"Impulse Idx (ns):",needSamples);
    field(PI_PROBABILITY,"Probability (p):",needProb);

    // Кнопка для генерации сигнала. При нажатии вызываем функцию генерации сигнала в состоянии приложения
    Rectangle genBtn = {x, y, fw, 28.0f};
    drawButton(genBtn, "[ Generate Signal ]", false);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mp = GetMousePosition();
        if (CheckCollisionPointRec(mp, genBtn)) {
            state.generateSignal();
        }
    }

    y += 34.0f; // Вертикальный отступ после кнопки генерации сигнала

    drawText("Histogram bins:", static_cast<int>(x), static_cast<int>(y), FONT_SZ, DARKGRAY);
    y += 18.0f;
    // Рисуем кнопки для выбора количества бинов в гистограмме. При нажатии на кнопку обновляем количество бинов в состоянии приложения
    int binOpts[4] = {5, 10, 15, 20}; 
    float bbw = (fw - 9.0f) / 4.0f; // Вычисляем ширину кнопок для выбора количества бинов с учетом отступов
    for (int i = 0; i < 4; ++i) {
        char lbl[8]; 
        snprintf(lbl, sizeof(lbl), "%d", binOpts[i]);

        // Вычисляем прямоугольник для кнопки с учетом отступов и размера
        Rectangle br = { x + i * (bbw + 3), y, bbw, bh };

        drawButton(br, lbl, state.histBins == binOpts[i]);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mp = GetMousePosition();
            if (CheckCollisionPointRec(mp, br)) {
                state.histBins = binOpts[i];
            }
        }
    }

    y += bh + 8.0f; // Вертикальный отступ после блока выбора количества бинов

    // Рисуем блок со статистикой сигнала. Если статистика недоступна, показываем подсказку пользователю
    Rectangle statsRect = {x, y, fw, 100.0f};
    DrawRectangleRec(statsRect, {220, 230, 220, 255});
    DrawRectangleLinesEx(statsRect, 1.0f, COL_BTN_BORDER);
    {
        float tx = statsRect.x + 4, ty = statsRect.y + 4; // Координаты для текста внутри блока статистики
        const int fs = 13;
        if (!state.statsValid) {
            drawText("No stats - generate a signal first",
                     static_cast<int>(tx), static_cast<int>(ty), fs, GRAY);
        } else {
            char buf[64];
            snprintf(buf, sizeof(buf), "Mean:     %+.5g", state.statMean);
            drawText(buf, static_cast<int>(tx), static_cast<int>(ty), fs, BLACK); ty += fs+3;
            snprintf(buf, sizeof(buf), "|Mean|:   %.5g",  state.statAbsMean);
            drawText(buf, static_cast<int>(tx), static_cast<int>(ty), fs, BLACK); ty += fs+3;
            snprintf(buf, sizeof(buf), "RMS:      %.5g",  state.statRms);
            drawText(buf, static_cast<int>(tx), static_cast<int>(ty), fs, BLACK); ty += fs+3;
            snprintf(buf, sizeof(buf), "Variance: %.5g",  state.statVar);
            drawText(buf, static_cast<int>(tx), static_cast<int>(ty), fs, BLACK); ty += fs+3;
            snprintf(buf, sizeof(buf), "Power:    %.5g",  state.statPower);
            drawText(buf, static_cast<int>(tx), static_cast<int>(ty), fs, BLACK);
        }
    }

    y += 108.0f;

    drawText("File:", static_cast<int>(x), static_cast<int>(y), FONT_SZ, DARKGRAY);
    y += 18.0f;

    // Рисуем кнопки для сохранения и загрузки сигнала. При нажатии на кнопку открываем соответствующий диалог в состоянии приложения
    float hw = (fw - 4.0f) / 2.0f;
    Rectangle saveBtn = {x,      y, hw, 26.0f};
    Rectangle loadBtn = {x+hw+4, y, hw, 26.0f};

    drawButton(saveBtn, "Save Signal", false);
    drawButton(loadBtn, "Load Signal", false);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mp = GetMousePosition();
        if (CheckCollisionPointRec(mp, saveBtn)) {
            state.showSaveDialog = true;
            memset(state.fileNameInput.buf, 0, sizeof(state.fileNameInput.buf));
            strncpy(state.fileNameInput.buf, "signals/", sizeof(state.fileNameInput.buf) - 1);
        }
        if (CheckCollisionPointRec(mp, loadBtn)) {
            state.showLoadDialog = true;
            memset(state.fileNameInput.buf, 0, sizeof(state.fileNameInput.buf));
            strncpy(state.fileNameInput.buf, "signals/", sizeof(state.fileNameInput.buf) - 1);
        }
    }
    y += 32.0f;

    drawText("Signal Operations:", static_cast<int>(x), static_cast<int>(y), FONT_SZ, DARKGRAY);
    y += 18.0f;

    {
        // Рисуем блок для загрузки первого сигнала для операций. Показываем путь к файлу, если он загружен, и кнопку для загрузки
        char lbl[80];
        snprintf(lbl, sizeof(lbl), "Sig1: %s",
                 state.opFile1Path.empty() ? "(none)" : state.opFile1Path.c_str());

        Rectangle r  = {x, y, fw - 54.0f, 22.0f};
        Rectangle lb = {x + fw - 52.0f, y, 52.0f, 22.0f};

        DrawRectangleRec(r, WHITE);
        DrawRectangleLinesEx(r, 1.0f, COL_BTN_BORDER);
        
        drawText(lbl, static_cast<int>(r.x+3), static_cast<int>(r.y+4), 11, DARKGRAY);
        drawButton(lb, "Load", false);
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mp = GetMousePosition();
            // Если нажата кнопка загрузки для первого сигнала, открываем диалог загрузки и очищаем буфер ввода имени файла
            if (CheckCollisionPointRec(mp, lb)) {
                state.showOp1Dialog = true;
                memset(state.fileNameInput.buf, 0, sizeof(state.fileNameInput.buf));
                strncpy(state.fileNameInput.buf, "signals/", sizeof(state.fileNameInput.buf) - 1);
            }
        }
        y += 26.0f;
    }
    {
        // Рисуем блок для загрузки второго сигнала для операций. Показываем путь к файлу, если он загружен, и кнопку для загрузки
        char lbl[80];
        snprintf(lbl, sizeof(lbl), "Sig2: %s",
                 state.opFile2Path.empty() ? "(none)" : state.opFile2Path.c_str());

        Rectangle r  = {x, y, fw - 54.0f, 22.0f};
        Rectangle lb = {x + fw - 52.0f, y, 52.0f, 22.0f};
        
        DrawRectangleRec(r, WHITE);
        DrawRectangleLinesEx(r, 1.0f, COL_BTN_BORDER);
        
        drawText(lbl, static_cast<int>(r.x+3), static_cast<int>(r.y+4), 11, DARKGRAY);
        drawButton(lb, "Load", false);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mp = GetMousePosition();
            // Если нажата кнопка загрузки для второго сигнала, открываем диалог загрузки и очищаем буфер ввода имени файла
            if (CheckCollisionPointRec(mp, lb)) {
                state.showOp2Dialog = true;
                memset(state.fileNameInput.buf, 0, sizeof(state.fileNameInput.buf));
                strncpy(state.fileNameInput.buf, "signals/", sizeof(state.fileNameInput.buf) - 1);
            }
        }
        y += 26.0f;
    }

    // Рисуем кнопки для выбора операции над сигналами. При нажатии на кнопку обновляем выбранную операцию в состоянии приложения
    const char* opLabels[4] = {"+", "-", "x", "/"};
    float opbw = (fw - 9.0f) / 4.0f;
    for (int i = 0; i < 4; ++i) {
        Rectangle br = {x + i * (opbw + 3), y, opbw, bh};
        drawButton(br, opLabels[i], state.selectedOp == static_cast<Operation>(i));
        // Если нажата кнопка операции, обновляем выбранную операцию в состоянии приложения
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mp = GetMousePosition();
            if (CheckCollisionPointRec(mp, br)) {
                state.selectedOp = static_cast<Operation>(i);
            }
        }
    }
    y += bh + 6.0f;

    // Кнопка для выполнения выбранной операции над сигналами
    // Кнопка активна только если оба сигнала загружены и не пустые
    // При нажатии вызываем функцию обработки операции в состоянии приложения
    bool canCompute = state.opSignal1 && !state.opSignal1->samples.empty() &&
                      state.opSignal2 && !state.opSignal2->samples.empty();
    
    Rectangle computeBtn = {x, y, fw, 26.0f};
    drawButton(computeBtn, "[ Compute ]", canCompute);
    if (canCompute && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mp = GetMousePosition();
        if (CheckCollisionPointRec(mp, computeBtn)) {
            processSignalOperation(state);
        }
    }
    
    // Если операция недоступна, показываем подсказку пользователю, что нужно загрузить оба сигнала для вычисления
    if (!canCompute && !state.opFile1Path.empty() && state.opFile2Path.empty()) {
        drawText("Load both signals to compute",
                 static_cast<int>(x), static_cast<int>(y + 30), 11, GRAY);
    }
    y += 32.0f;

    // Рисуем статусное сообщение в нижней части панели. Если сообщение содержит слово Error - отображаем его красным цветом, иначе - зеленым
    if (!state.statusMsg.empty()) {
        Color col = (state.statusMsg.find("Error") != std::string::npos ||
                     state.statusMsg.find("error") != std::string::npos) ? RED : DARKGREEN;
        drawText(state.statusMsg.c_str(),
                 static_cast<int>(x), static_cast<int>(y), 12, col);
    }
}
