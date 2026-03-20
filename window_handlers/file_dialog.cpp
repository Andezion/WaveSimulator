#include "file_dialog.h"
#include "ui_font.h"
#include "ui_widgets.h"
#include <cstring>

void drawFileDialog(AppState& state, const char* title,
                    bool& showFlag,
                    std::function<void(const std::string&)> onOk) {
    if (!showFlag) {
        return;
    }

    // Рисуем полупрозрачный фон для затемнения остальной части интерфейса
    Rectangle dlg = {280, 240, 520, 140};
    DrawRectangleRec(dlg, Color{240, 240, 240, 255});
    DrawRectangleLinesEx(dlg, 2.0f, DARKGRAY);
    drawText(title, static_cast<int>(dlg.x + 10), static_cast<int>(dlg.y + 10), FONT_SZ, BLACK);

    // Рисуем поле ввода для пути к файлу и обрабатываем ввод текста
    // Принимаем текст из буфера ввода в состоянии приложения и отображаем его в поле
    // Также обрабатываем нажатия клавиш для редактирования текста
    TextInput& ti = state.fileNameInput;
    Rectangle inp = {dlg.x + 10, dlg.y + 40, dlg.width - 20, 28};
    DrawRectangleRec(inp, WHITE);
    DrawRectangleLinesEx(inp, 1.5f, BLUE);
    drawText(ti.buf,
             static_cast<int>(inp.x + 4),
             static_cast<int>(inp.y + 6),
             FONT_SZ, BLACK);

    handleTextInputKey(ti);

    // Рисуем кнопки OK и Cancel для подтверждения или отмены действия
    // При нажатии на OK вызываем функцию-обработчик с введенным путем к файлу, при нажатии на Cancel просто закрываем диалог
    Rectangle okBtn = {dlg.x + 10, dlg.y + 95, 90, 30};
    Rectangle canBtn = {dlg.x + dlg.width - 100, dlg.y + 95, 90, 30};
    drawButton(okBtn,"OK",false);
    drawButton(canBtn,"Cancel",false);

    // Обрабатываем нажатия мыши для кнопок OK и Cancel, а также нажатия клавиш Enter и Escape для подтверждения или отмены действия
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mp = GetMousePosition(); // Получаем позицию мыши при нажатии
        if (CheckCollisionPointRec(mp, okBtn) && strlen(ti.buf) > 0) {
            onOk(std::string(ti.buf));
            memset(ti.buf, 0, sizeof(ti.buf));
            showFlag = false;
        }
        if (CheckCollisionPointRec(mp, canBtn)) {
            memset(ti.buf, 0, sizeof(ti.buf));
            showFlag = false;
        }
    }
    if (IsKeyPressed(KEY_ENTER) && strlen(ti.buf) > 0) {
        onOk(std::string(ti.buf));
        memset(ti.buf, 0, sizeof(ti.buf));
        showFlag = false;
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        memset(ti.buf, 0, sizeof(ti.buf));
        showFlag = false;
    }
}
