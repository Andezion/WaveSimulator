#include "ui_widgets.h"
#include "ui_font.h"
#include <cstring>
#include <cmath>

// Тут рисуем кнопки и текстовые поля 
// Принимает координаты, подпись и состояние кнопки
void drawButton(Rectangle r, const char* label, bool active) {
    Color bg = active ? COL_BTN_ACTIVE : COL_BTN_NORMAL; // Цвет кнопки в зависимости от состояния

    DrawRectangleRec(r, bg); // Рисуем прямоугольник кнопки
    DrawRectangleLinesEx(r, 1.0f, COL_BTN_BORDER); // Рисуем границу кнопки

    // Вычисляем ширину текста для центрирования
    int tw = measureText(label, FONT_SZ);
    drawText(label,
             static_cast<int>(r.x + (r.width  - tw) / 2),
             static_cast<int>(r.y + (r.height - FONT_SZ) / 2),
             FONT_SZ, active ? DARKBLUE : DARKGRAY);
}

// Обрабатываем ввод текста в текстовом поле
// Принимает ссылку на структуру TextInput, которая содержит буфер для текста
void handleTextInputKey(TextInput& ti) {
    int ch; // Переменная для хранения нажатого символа
    // Получаем все нажатые символы и добавляем их в буфер, если это допустимые символы
    while ((ch = GetCharPressed()) != 0) {
        int len = static_cast<int>(strlen(ti.buf));
        if (ch >= 32 && len < 62) {
            ti.buf[len]   = static_cast<char>(ch);
            ti.buf[len+1] = '\0';
        }
    }
    // Обрабатываем нажатие клавиши Backspace для удаления последнего символа из буфера
    if (IsKeyPressed(KEY_BACKSPACE)) {
        int len = static_cast<int>(strlen(ti.buf));
        if (len > 0) {
            ti.buf[len - 1] = '\0';
        }
    }
}

// Рисуем текстовое поле для ввода параметров
// Принимает состояние приложения, индекс текстового поля, координаты и подпись
void drawTextInput(AppState& state, int idx, Rectangle r, const char* label) {
    TextInput& ti = state.paramInputs[static_cast<size_t>(idx)]; // Получаем ссылку на структуру TextInput для данного индекса
    bool focused  = (state.focusedInput == idx); // Проверяем является ли это текстовое поле активным (в фокусе)

    // Рисуем подпись над текстовым полем
    drawText(label,
             static_cast<int>(r.x),
             static_cast<int>(r.y - 16),
             FONT_SZ, DARKGRAY);

    DrawRectangleRec(r, focused ? Color{255, 255, 220, 255} : WHITE); // Рисуем прямоугольник текстового поля
    DrawRectangleLinesEx(r, 1.0f, focused ? BLUE : COL_BTN_BORDER); // Рисуем границу текстового поля
    
    drawText(ti.buf,
             static_cast<int>(r.x + 4),
             static_cast<int>(r.y + (r.height - FONT_SZ) / 2),
             FONT_SZ, BLACK);
    // Если текстовое поле в фокусе, рисуем мигающий курсор и обрабатываем ввод текста
    if (focused) {
        double t = GetTime(); // Получаем текущее время для мигания курсора
        // Рисуем вертикальную линию курсора, которая мигает каждые 0.5 секунды
        if ((t - std::floor(t)) < 0.5) {
            // Вычисляем позицию курсора, которая находится после последнего символа в буфере
            int cx = static_cast<int>(r.x + 4) + measureText(ti.buf, FONT_SZ);
            DrawLine(cx, static_cast<int>(r.y + 4),
                     cx, static_cast<int>(r.y + r.height - 4), BLACK);
        }
        // Обрабатываем ввод текста для данного текстового поля
        handleTextInputKey(ti);
    }
    // Обрабатываем клик мыши для установки фокуса на текстовое поле
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mp = GetMousePosition();
        if (CheckCollisionPointRec(mp, r)) {
            state.focusedInput = idx;
        }
    }
}
