#pragma once
#include <raylib.h>
#include "../app_state.h"

void drawButton(Rectangle r, const char* label, bool active);
void handleTextInputKey(TextInput& ti);
void drawTextInput(AppState& state, int idx, Rectangle r, const char* label);
