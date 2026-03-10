#include "raylib.h"
#include "app_state.h"
#include "window_handlers/draw_window.h"
#include <cstdlib>
#include <ctime>

void processInput(AppState& state, int plotX, int plotW);
void clampPlotScroll(AppState& state, float plotDrawWidth);

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    const int SCREEN_W = 1400;
    const int SCREEN_H = 800;
    const int PANEL_W  = 320;
    const int PLOT_PAD_L = 55; 
    const int PLOT_PAD_R = 10;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_W, SCREEN_H, "WaveSimulator — Digital Signal Processing");
    SetTargetFPS(60);

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

    CloseWindow();
    return 0;
}
