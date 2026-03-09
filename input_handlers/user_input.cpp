#include "../app_state.h"
#include "../waves_math/signal.h"
#include <raylib.h>
#include <algorithm>
#include <cstring>

void processSignalOperation(AppState& state) {
    if (!state.opSignal1 || state.opSignal1->samples.empty()) {
        state.statusMsg = "Error: Signal 1 not loaded";
        return;
    }
    if (!state.opSignal2 || state.opSignal2->samples.empty()) {
        state.statusMsg = "Error: Signal 2 not loaded";
        return;
    }

    size_t len = std::min(state.opSignal1->samples.size(),
                          state.opSignal2->samples.size());

    auto result = std::make_unique<GenericSignal>();
    result->name = "Result Signal";
    result->params = state.opSignal1->params;
    result->samples.resize(len);
    result->times.resize(len);

    for (size_t i = 0; i < len; ++i) {
        result->times[i] = state.opSignal1->times[i];

        double a = state.opSignal1->samples[i];
        double b = state.opSignal2->samples[i];

        switch (state.selectedOp) {
            case Operation::Add: result->samples[i] = a + b; break;
            case Operation::Sub: result->samples[i] = a - b; break;
            case Operation::Mul: result->samples[i] = a * b; break;
            case Operation::Div:
                result->samples[i] = (b != 0.0) ? a / b : 0.0;
                break;
        }
    }

    state.resultSignal = std::move(result);
    state.updateStats();
    state.plotScrollX = 0.0f;
    state.statusMsg = "Operation applied successfully";
}

void processInput(AppState& state, int plotX, int plotW) {
    Vector2 mouse = GetMousePosition();
    bool overPlot = (mouse.x >= plotX && mouse.x <= plotX + plotW);

    if (overPlot) {
        float wheel = GetMouseWheelMove();
        if (wheel != 0.0f) {
            bool ctrl = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
            if (ctrl) {
                state.plotZoom *= (wheel > 0 ? 1.15f : 1.0f / 1.15f);
                if (state.plotZoom < 0.1f)  state.plotZoom = 0.1f;
                if (state.plotZoom > 100.0f) state.plotZoom = 100.0f;
            } else {
                state.plotScrollX -= wheel * 30.0f;
                if (state.plotScrollX < 0.0f) state.plotScrollX = 0.0f;
            }
        }
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        state.focusedInput = -1;
        
        state.showSaveDialog = false;
        state.showLoadDialog = false;
        state.showOp1Dialog  = false;
        state.showOp2Dialog  = false;
    }
}
