#include "ui_font.h"

static Font g_font = {};
static bool g_fontLoaded = false;

void initDrawFont() {
    g_font = LoadFontEx("/usr/share/fonts/liberation-sans-fonts/LiberationSans-Regular.ttf",
                        20, nullptr, 0);
    g_fontLoaded = (g_font.texture.id > 0);
}

void cleanupDrawFont() {
    if (g_fontLoaded) {
        UnloadFont(g_font);
    }
}

void drawText(const char* text, int x, int y, int size, Color color) {
    if (g_fontLoaded) {
        DrawTextEx(g_font, text, {(float)x, (float)y}, (float)size, 1.0f, color);
    } else {
        DrawText(text, x, y, size, color);
    }
}

int measureText(const char* text, int size) {
    if (g_fontLoaded) {
        return static_cast<int>(MeasureTextEx(g_font, text, static_cast<float>(size), 1.0f).x);
    }
    return MeasureText(text, size);
}
