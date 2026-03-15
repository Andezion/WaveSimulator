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
    // Отступ справа для графика (для оси и отрисовки текста)
    const int PLOT_PAD_R = 10;

    // Разрешаем изменять размер окна
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    // Инициализация окна
    InitWindow(SCREEN_W, SCREEN_H, "WaveSimulator");
    
    // Установка целевой частоты кадров для плавной отрисовки
    SetTargetFPS(60);
    // Инициализация шрифтов для отрисовки текста
    initDrawFont();

    AppState state;
    // Инициализация состояния приложения (выбранный тип сигнала, параметры и так далее)
    state.init();

    // Начинаем работу
    while (!WindowShouldClose()) {
        // Вычисляем размеры правой области для отрисовки графика и гистограммы
        int plotX = PANEL_W + PLOT_PAD_L;
        // Ширина области для отрисовки графика
        int plotW = GetScreenWidth() - PANEL_W - PLOT_PAD_L - PLOT_PAD_R;

        // Обрабатываем ввод пользователя (клики мыши, нажатия клавиш и так далее)
        processInput(state, plotX, plotW);
        // Ограничиваем прокрутку графика, чтобы не было провалов при скролле
        clampPlotScroll(state, static_cast<float>(plotW));

        // Начинаем отрисовку текущего кадра
        BeginDrawing();
        // Отрисовываем все элементы интерфейса и графики
            drawWindow(state);
        // Конец отрисовки текущего кадра
        EndDrawing();
    }

    // Очистка ресурсов шрифтов перед закрытием приложения
    cleanupDrawFont();
    // Закрытие окна и освобождение ресурсов
    CloseWindow();
    return 0;
}
