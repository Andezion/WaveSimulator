#include "raylib.h"
#include "app_state.h"
#include "window_handlers/draw_window.h"
#include <cstdlib>
#include <ctime>

// Функции для обработки ввода и ограничения прокрутки
void processInput(AppState& state, int plotX, int plotW);
// Функция для ограничения прокрутки графика
void clampPlotScroll(AppState& state, float plotDrawWidth);

int main() {
    // Инициализация генератора случайных чисел для шумов
    srand(static_cast<unsigned>(time(nullptr)));

    const int SCREEN_W = 1400;
    const int SCREEN_H = 800;
    // Ширина левой панели
    const int PANEL_W = 320;
    // Отступы для графика внутри правой области
    const int PLOT_PAD_L = 55; 
    const int PLOT_PAD_R = 10;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_W, SCREEN_H, "WaveSimulator");
    
    SetTargetFPS(60);
    initDrawFont();

    AppState state;
    state.init();

    while (!WindowShouldClose()) {
        int plotX = PANEL_W + PLOT_PAD_L;
        int plotW = GetScreenWidth() - PANEL_W - PLOT_PAD_L - PLOT_PAD_R;

        processInput(state, plotX, plotW);
        clampPlotScroll(state, static_cast<float>(plotW));

        BeginDrawing();
            drawWindow(state);
        EndDrawing();
    }

    cleanupDrawFont();
    CloseWindow();
    return 0;
}
