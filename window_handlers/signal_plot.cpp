#include "signal_plot.h"
#include "ui_font.h"
#include <algorithm>
#include <string>

// Рисуем график сигнала в заданной области
void drawSignalPlot(AppState& state, Rectangle plotRect) {
    // Рисуем фон и оси графика
    DrawRectangleRec(plotRect, COL_PLOT_BG);
    // Рисуем границу графика
    DrawRectangleLinesEx(plotRect, 1.0f, COL_AXIS);

    // Берем сигнал для отображения - это будет результат операции, если есть, иначе текущий сигнал
    Signal* sig = state.resultSignal  ? state.resultSignal.get()
                : state.currentSignal ? state.currentSignal.get()
                : nullptr;

    // Если сигнала нет или он пустой, показываем подсказку пользователю
    if (!sig || sig->samples.empty()) {
        drawText("No signal — press [ Generate Signal ]",
                 static_cast<int>(plotRect.x + 20),
                 static_cast<int>(plotRect.y + plotRect.height / 2 - 10),
                 FONT_SZ, GRAY);
        return;
    }

    // Параметры отступов для рисования графика внутри области
    const float PAD_L = 55.0f, PAD_R = 10.0f, PAD_T = 18.0f, PAD_B = 30.0f;

    // Вычисляем размеры области для рисования графика, учитывая отступы
    float drawW = plotRect.width  - PAD_L - PAD_R;
    float drawH = plotRect.height - PAD_T - PAD_B;

    // Вычисляем координаты начала области для рисования графика
    float ox = plotRect.x + PAD_L;
    float oy = plotRect.y + PAD_T;

    // Получаем количество отсчетов в сигнале
    int n = static_cast<int>(sig->samples.size());

    // Находим минимальное и максимальное значение сигнала для масштабирования по вертикали
    double yMin = *std::min_element(sig->samples.begin(), sig->samples.end());
    double yMax = *std::max_element(sig->samples.begin(), sig->samples.end());

    // Если сигнал почти плоский, добавляем небольшой запас к диапазону, чтобы график был виден
    if (yMax - yMin < 1e-12) { 
        yMin -= 1.0; 
        yMax += 1.0; 
    }
    double yRange = yMax - yMin;

    // Находим минимальное и максимальное время сигнала для масштабирования по горизонтали
    double tMin = sig->times.front();
    double tMax = sig->times.back();

    double tRange = tMax - tMin;
    if (tRange < 1e-12) {
        tRange = 1.0;
    }

    // Вычисляем масштаб и прокрутку для отображения нужного диапазона сигнала
    float zoom = state.plotZoom > 0.01f ? state.plotZoom : 1.0f;

    // Вычисляем видимый диапазон времени на графике с учетом масштаба и прокрутки
    float visibleDuration = static_cast<float>(tRange) / zoom;
    // Вычисляем, на сколько прокручено отображение сигнала влево или вправо, в зависимости от состояния прокрутки
    float scrollT = state.plotScrollX / drawW * static_cast<float>(tRange) / zoom;

    // Вычисляем минимальное и максимальное время, отображаемое на графике, с учетом прокрутки
    double tViewMin = tMin + scrollT;
    double tViewMax = tViewMin + visibleDuration;

    (void)tViewMax; // Пока не используем tViewMax, но он может пригодиться для оптимизации отрисовки!!!

    // Включаем режим отсечения, чтобы рисовать только внутри области графика
    BeginScissorMode(static_cast<int>(ox), static_cast<int>(oy),
                     static_cast<int>(drawW), static_cast<int>(drawH));

    // Лямбда-функции для преобразования координат времени и значения сигнала в экранные координаты
    auto toScreenX = [&](double t) -> float {
        return ox + static_cast<float>((t - tViewMin) / visibleDuration) * drawW;
    };
    // Преобразуем значение сигнала в экранные координаты по вертикали, учитывая масштабирование и отступы
    auto toScreenY = [&](double v) -> float {
        return oy + static_cast<float>(1.0 - (v - yMin) / yRange) * drawH;
    };

    // Рисуем горизонтальную линию для нулевого уровня сигнала, если он попадает в видимую область
    float zeroY = toScreenY(0.0);
    if (zeroY >= oy && zeroY <= oy + drawH) {
        DrawLine(static_cast<int>(ox), static_cast<int>(zeroY),
                 static_cast<int>(ox + drawW), static_cast<int>(zeroY),
                 Color{200, 200, 200, 255});
    }

    // Рисуем график сигнала. Если сигнал дискретный, рисуем вертикальные линии и кружки для каждого отсчета. Если непрерывный - рисуем ломаную линию
    if (sig->isDiscrete()) {
        for (int i = 0; i < n; ++i) {
            float sx = toScreenX(sig->times[static_cast<size_t>(i)]);
            float sy = toScreenY(sig->samples[static_cast<size_t>(i)]);

            if (sx < ox - 2 || sx > ox + drawW + 2) continue;
            DrawLine(static_cast<int>(sx), static_cast<int>(zeroY),
                     static_cast<int>(sx), static_cast<int>(sy), COL_SIGNAL);
            DrawCircle(static_cast<int>(sx), static_cast<int>(sy), 3.0f, COL_SIGNAL);
        }
    } else {
        for (int i = 1; i < n; ++i) {
            float x1 = toScreenX(sig->times[static_cast<size_t>(i-1)]);
            float y1 = toScreenY(sig->samples[static_cast<size_t>(i-1)]);
            float x2 = toScreenX(sig->times[static_cast<size_t>(i)]);
            float y2 = toScreenY(sig->samples[static_cast<size_t>(i)]);

            if (x2 < ox - 1 || x1 > ox + drawW + 1) continue;
            DrawLine(static_cast<int>(x1), static_cast<int>(y1),
                     static_cast<int>(x2), static_cast<int>(y2), COL_SIGNAL);
        }
    }

    // Завершаем режим отсечения, чтобы рисовать UI элементы поверх графика
    EndScissorMode();

    // Рисуем оси и подписи к ним. Для вертикальной оси рисуем 5 делений, для горизонтальной - 6 делений, с учетом видимого диапазона времени
    for (int i = 0; i <= 5; ++i) {
        double v  = yMin + yRange * i / 5.0;
        float  sy = toScreenY(v);

        if (sy < oy || sy > oy + drawH) {
            continue;
        }

        char lbl[20]; 
        snprintf(lbl, sizeof(lbl), "%.3g", v);

        int tw = measureText(lbl, 11);

        drawText(lbl, static_cast<int>(ox - tw - 3), static_cast<int>(sy - 6), 11, DARKGRAY);
        DrawLine(static_cast<int>(ox - 3), static_cast<int>(sy),
                 static_cast<int>(ox),     static_cast<int>(sy), COL_AXIS);
    }

    // Рисуем деления и подписи для горизонтальной оси, учитывая видимый диапазон времени
    for (int i = 0; i <= 6; ++i) {
        double t  = tViewMin + visibleDuration * i / 6.0;
        float  sx = toScreenX(t);

        char lbl[20]; 
        snprintf(lbl, sizeof(lbl), "%.3g", t);

        int tw = measureText(lbl, 11);

        drawText(lbl, static_cast<int>(sx - tw/2.0f),
                 static_cast<int>(oy + drawH + 4), 11, DARKGRAY);
        DrawLine(static_cast<int>(sx), static_cast<int>(oy + drawH),
                 static_cast<int>(sx), static_cast<int>(oy + drawH + 3), COL_AXIS);
    }

    // Рисуем рамки и подписи для осей графика
    DrawLine(static_cast<int>(ox), static_cast<int>(oy),
             static_cast<int>(ox), static_cast<int>(oy + drawH), COL_AXIS);
    DrawLine(static_cast<int>(ox), static_cast<int>(oy + drawH),
             static_cast<int>(ox + drawW), static_cast<int>(oy + drawH), COL_AXIS);

    
    std::string titleStr = state.resultSignal  ? "Result Signal"
                         : state.currentSignal ? state.currentSignal->getName()
                         : std::string{};
    drawText(titleStr.c_str(), static_cast<int>(ox + 4), static_cast<int>(oy + 2), 12, DARKBLUE);

    // Рисуем подсказку для управления графиком, если сигнал есть
    drawText("Mouse wheel: scroll  |  Ctrl+wheel: zoom",
             static_cast<int>(ox + drawW - 240),
             static_cast<int>(oy + drawH + 16), 10, GRAY);
}
