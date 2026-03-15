#include "draw_window.h"
#include "ui_font.h"
#include "left_panel.h"
#include "signal_plot.h"
#include "histogram_plot.h"
#include "file_dialog.h"
#include "../file_handlers/file_io.h"

void drawWindow(AppState& state) {
    ClearBackground(LIGHTGRAY);

    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();
    int rightX = PANEL_W;
    int rightW = screenW - PANEL_W;
    int plotH = static_cast<int>(screenH * 0.60f);
    int histH = screenH - plotH;

    drawLeftPanel(state);

    Rectangle plotRect = {
        static_cast<float>(rightX), 0.0f,
        static_cast<float>(rightW), static_cast<float>(plotH)
    };
    Rectangle histRect = {
        static_cast<float>(rightX), static_cast<float>(plotH),
        static_cast<float>(rightW), static_cast<float>(histH)
    };

    drawSignalPlot(state, plotRect);
    drawHistogram(state,  histRect);

    drawFileDialog(state, "Save Signal — enter filename:", state.showSaveDialog,
        [&](const std::string& path) {
            if (state.currentSignal) {
                bool ok = saveSignal(*state.currentSignal, path);
                state.statusMsg = ok ? "Saved: " + path : "Error saving: " + path;
            } else {
                state.statusMsg = "No signal to save";
            }
        });

    drawFileDialog(state, "Load Signal — enter filename:", state.showLoadDialog,
        [&](const std::string& path) {
            auto s = loadSignal(path);
            if (s) {
                state.currentSignal = std::move(s);
                state.updateStats();
                state.plotScrollX = 0.0f;
                state.statusMsg = "Loaded: " + path;
            } else {
                state.statusMsg = "Error loading: " + path;
            }
        });

    drawFileDialog(state, "Load Signal 1 — enter filename:", state.showOp1Dialog,
        [&](const std::string& path) {
            auto s = loadSignal(path);
            if (s) {
                state.opSignal1   = std::move(s);
                state.opFile1Path = path;
                state.statusMsg   = "Signal 1 loaded";
            } else {
                state.statusMsg = "Error loading signal 1: " + path;
            }
        });

    drawFileDialog(state, "Load Signal 2 — enter filename:", state.showOp2Dialog,
        [&](const std::string& path) {
            auto s = loadSignal(path);
            if (s) {
                state.opSignal2   = std::move(s);
                state.opFile2Path = path;
                state.statusMsg   = "Signal 2 loaded";
            } else {
                state.statusMsg = "Error loading signal 2: " + path;
            }
        });
}
