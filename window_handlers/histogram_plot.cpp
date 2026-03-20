#include "histogram_plot.h"
#include "ui_font.h"
#include <algorithm>
#include <vector>
#include <cstdio>

void drawHistogram(AppState& state, Rectangle histRect) {
    // Рисуем фон и границу для гистограммы
    DrawRectangleRec(histRect, Color{245, 245, 255, 255});
    DrawRectangleLinesEx(histRect, 1.0f, COL_AXIS);

    // Выбираем сигнал для отображения гистограммы - это будет результат операции, если он есть, иначе текущий сигнал
    Signal* sig = state.resultSignal  ? state.resultSignal.get()
                : state.currentSignal ? state.currentSignal.get()
                : nullptr;

    // Если сигнала нет или он пустой, показываем подсказку пользователю
    if (!sig || sig->samples.empty()) {
        drawText("Histogram: no signal",
                 static_cast<int>(histRect.x + 10),
                 static_cast<int>(histRect.y + 10),
                 FONT_SZ, GRAY);
        return;
    }

    // Параметры отступов для рисования гистограммы внутри области
    const float PAD_L = 55.0f, PAD_R = 10.0f, PAD_T = 18.0f, PAD_B = 30.0f;

    // Вычисляем размеры области для рисования гистограммы, учитывая отступы
    float drawW = histRect.width  - PAD_L - PAD_R;
    float drawH = histRect.height - PAD_T - PAD_B;

    // Вычисляем координаты начала области для рисования гистограммы
    float ox = histRect.x + PAD_L;
    float oy = histRect.y + PAD_T;

    // Вычисляем количество бинов для гистограммы, используя значение из состояния приложения. Если значение некорректное, используем 10 бинов по умолчанию
    int bins = state.histBins > 0 ? state.histBins : 10;

    // Находим минимальное и максимальное значение сигнала для определения диапазона гистограммы
    double yMin = *std::min_element(sig->samples.begin(), sig->samples.end());
    double yMax = *std::max_element(sig->samples.begin(), sig->samples.end());

    if (yMax - yMin < 1e-12) { 
        yMin -= 1.0; 
        yMax += 1.0; 
    }

    // Вычисляем ширину бина для гистограммы на основе диапазона значений сигнала и количества бинов
    double binW = (yMax - yMin) / bins;

    // Считаем количество отсчетов сигнала, попадающих в каждый бин гистограммы
    std::vector<int> counts(static_cast<size_t>(bins), 0);
    for (double v : sig->samples) {
        int b = static_cast<int>((v - yMin) / binW);
        if (b < 0) {
            b = 0;
        }
        if (b >= bins) {
            b = bins - 1;
        }
        counts[static_cast<size_t>(b)]++;
    }

    int maxCount = *std::max_element(counts.begin(), counts.end());
    if (maxCount == 0) {
        maxCount = 1;
    }

    float bw = drawW / bins;
    for (int i = 0; i < bins; ++i) {
        float barH = static_cast<float>(counts[static_cast<size_t>(i)]) / maxCount * drawH;
        Rectangle bar = { ox + i * bw + 1.0f, oy + drawH - barH, bw - 2.0f, barH };
        DrawRectangleRec(bar, COL_HIST);
        DrawRectangleLinesEx(bar, 1.0f, Color{50, 100, 180, 255});
    }

    for (int i = 0; i <= bins; ++i) {
        double v = yMin + i * binW;
        float sx = ox + i * bw;

        char lbl[20]; snprintf(lbl, sizeof(lbl), "%.2g", v);
        int tw = measureText(lbl, 10);

        drawText(lbl, static_cast<int>(sx - tw/2.0f),
                 static_cast<int>(oy + drawH + 3), 10, DARKGRAY);
    }

    for (int i = 0; i <= 4; ++i) {
        int cnt = maxCount * i / 4;
        float sy = oy + drawH - static_cast<float>(cnt) / maxCount * drawH;

        char lbl[16]; snprintf(lbl, sizeof(lbl), "%d", cnt);
        int tw = measureText(lbl, 10);

        drawText(lbl, static_cast<int>(ox - tw - 3), static_cast<int>(sy - 5), 10, DARKGRAY);
        DrawLine(static_cast<int>(ox - 3), static_cast<int>(sy),
                 static_cast<int>(ox), static_cast<int>(sy), COL_AXIS);
    }

    DrawLine(static_cast<int>(ox), static_cast<int>(oy),
             static_cast<int>(ox), static_cast<int>(oy + drawH), COL_AXIS);
    DrawLine(static_cast<int>(ox), static_cast<int>(oy + drawH),
             static_cast<int>(ox + drawW), static_cast<int>(oy + drawH), COL_AXIS);

    drawText("Histogram", static_cast<int>(ox + 4), static_cast<int>(oy + 2), 12, DARKBLUE);
}
