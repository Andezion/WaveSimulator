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

    Rectangle dlg = {280, 240, 520, 140};
    DrawRectangleRec(dlg, Color{240, 240, 240, 255});
    DrawRectangleLinesEx(dlg, 2.0f, DARKGRAY);
    drawText(title, static_cast<int>(dlg.x + 10), static_cast<int>(dlg.y + 10), FONT_SZ, BLACK);

    TextInput& ti = state.fileNameInput;
    Rectangle inp = {dlg.x + 10, dlg.y + 40, dlg.width - 20, 28};
    DrawRectangleRec(inp, WHITE);
    DrawRectangleLinesEx(inp, 1.5f, BLUE);
    drawText(ti.buf,
             static_cast<int>(inp.x + 4),
             static_cast<int>(inp.y + 6),
             FONT_SZ, BLACK);

    handleTextInputKey(ti);

    Rectangle okBtn = {dlg.x + 10, dlg.y + 95, 90, 30};
    Rectangle canBtn = {dlg.x + dlg.width - 100, dlg.y + 95, 90, 30};
    drawButton(okBtn,"OK",false);
    drawButton(canBtn,"Cancel",false);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mp = GetMousePosition();
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
