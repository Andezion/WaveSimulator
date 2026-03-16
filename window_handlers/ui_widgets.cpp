#include "ui_widgets.h"
#include "ui_font.h"
#include <cstring>
#include <cmath>

// Тут рисуем кнопки и текстовые поля 
// Принимает координаты, подпись и состояние кнопки
void drawButton(Rectangle r, const char* label, bool active) {
    Color bg = active ? COL_BTN_ACTIVE : COL_BTN_NORMAL; // Цвет кнопки в зависимости от состояния

    DrawRectangleRec(r, bg);
    DrawRectangleLinesEx(r, 1.0f, COL_BTN_BORDER);

    int tw = measureText(label, FONT_SZ);
    drawText(label,
             static_cast<int>(r.x + (r.width  - tw) / 2),
             static_cast<int>(r.y + (r.height - FONT_SZ) / 2),
             FONT_SZ, active ? DARKBLUE : DARKGRAY);
}

void handleTextInputKey(TextInput& ti) {
    int ch;
    while ((ch = GetCharPressed()) != 0) {
        int len = static_cast<int>(strlen(ti.buf));
        if (ch >= 32 && len < 62) {
            ti.buf[len]   = static_cast<char>(ch);
            ti.buf[len+1] = '\0';
        }
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
        int len = static_cast<int>(strlen(ti.buf));
        if (len > 0) {
            ti.buf[len - 1] = '\0';
        }
    }
}

void drawTextInput(AppState& state, int idx, Rectangle r, const char* label) {
    TextInput& ti = state.paramInputs[static_cast<size_t>(idx)];
    bool focused  = (state.focusedInput == idx);

    drawText(label,
             static_cast<int>(r.x),
             static_cast<int>(r.y - 16),
             FONT_SZ, DARKGRAY);

    DrawRectangleRec(r, focused ? Color{255, 255, 220, 255} : WHITE);
    DrawRectangleLinesEx(r, 1.0f, focused ? BLUE : COL_BTN_BORDER);
    drawText(ti.buf,
             static_cast<int>(r.x + 4),
             static_cast<int>(r.y + (r.height - FONT_SZ) / 2),
             FONT_SZ, BLACK);

    if (focused) {
        double t = GetTime();
        if ((t - std::floor(t)) < 0.5) {
            int cx = static_cast<int>(r.x + 4) + measureText(ti.buf, FONT_SZ);
            DrawLine(cx, static_cast<int>(r.y + 4),
                     cx, static_cast<int>(r.y + r.height - 4), BLACK);
        }
        handleTextInputKey(ti);
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mp = GetMousePosition();
        if (CheckCollisionPointRec(mp, r)) {
            state.focusedInput = idx;
        }
    }
}
