#include "../app_state.h"

// Функция для ограничения прокрутки графика сигнала, чтобы она не выходила за пределы допустимых значений
void clampPlotScroll(AppState& state, float plotDrawWidth) {
    if (plotDrawWidth <= 0.0f) {
        return;
    }
    
    if (!state.currentSignal || state.currentSignal->samples.empty()) {
        state.plotScrollX = 0.0f;
        return;
    }
    float maxScroll = plotDrawWidth * (state.plotZoom - 1.0f);

    if (maxScroll < 0.0f) {
        maxScroll = 0.0f;
    }

    if (state.plotScrollX > maxScroll) { 
        state.plotScrollX = maxScroll;
    }

    if (state.plotScrollX < 0.0f) {
        state.plotScrollX = 0.0f;
    }
}
