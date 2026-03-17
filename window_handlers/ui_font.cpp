#include "ui_font.h"

static Font g_font = {}; // Глобальная переменная для хранения шрифта
static bool g_fontLoaded = false; // Загрузили ли шрифт успешно

// Инициализация шрифта для рисования текста
void initDrawFont() {
    g_font = LoadFontEx("/usr/share/fonts/liberation-sans-fonts/LiberationSans-Regular.ttf",
                        20, nullptr, 0);
    // Проверяем, загрузился ли шрифт успешно
    g_fontLoaded = (g_font.texture.id > 0);
}

// Очистка ресурсов, связанных с шрифтом
void cleanupDrawFont() {
    if (g_fontLoaded) {
        UnloadFont(g_font);
    }
}

// Рисование текста с использованием шрифта
void drawText(const char* text, int x, int y, int size, Color color) {
    if (g_fontLoaded) {
        DrawTextEx(g_font, text, {(float)x, (float)y}, (float)size, 1.0f, color);
    } else {
        DrawText(text, x, y, size, color);
    }
}

// Измерение ширины текста для центрирования
int measureText(const char* text, int size) {
    if (g_fontLoaded) {
        return static_cast<int>(MeasureTextEx(g_font, text, static_cast<float>(size), 1.0f).x);
    }
    return MeasureText(text, size);
}
