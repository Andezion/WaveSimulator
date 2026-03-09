// raylib.h MUST come first — it defines Color, Rectangle, Vector2, etc.
#include <raylib.h>
#include "draw_window.h"
#include "../app_state.h"
#include "../file_handlers/file_io.h"
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <functional>
#include <string>

// ── colours ──────────────────────────────────────────────────────────────────
static const Color COL_PANEL      = {230, 230, 230, 255};
static const Color COL_PLOT_BG    = WHITE;
static const Color COL_SIGNAL     = RED;
static const Color COL_AXIS       = DARKGRAY;
static const Color COL_BTN_NORMAL = {245, 245, 245, 255};
static const Color COL_BTN_ACTIVE = {180, 210, 255, 255};
static const Color COL_BTN_BORDER = {160, 160, 160, 255};
static const Color COL_HIST       = { 80, 160, 220, 255};

// ── layout constants ──────────────────────────────────────────────────────────
static const int SCREEN_W = 1280;
static const int SCREEN_H = 720;
static const int PANEL_W  = 320;
static const int RIGHT_X  = PANEL_W;
static const int RIGHT_W  = SCREEN_W - PANEL_W;
static const int PLOT_H   = static_cast<int>(SCREEN_H * 0.60f);
static const int HIST_H   = SCREEN_H - PLOT_H;
static const int FONT_SZ  = 14;

// forward declaration for operation handler in user_input.cpp
void processSignalOperation(AppState& state);

// ── helpers ───────────────────────────────────────────────────────────────────
static void drawButton(Rectangle r, const char* label, bool active) {
    Color bg = active ? COL_BTN_ACTIVE : COL_BTN_NORMAL;
    DrawRectangleRec(r, bg);
    DrawRectangleLinesEx(r, 1.0f, COL_BTN_BORDER);
    int tw = MeasureText(label, FONT_SZ);
    DrawText(label,
             static_cast<int>(r.x + (r.width  - tw) / 2),
             static_cast<int>(r.y + (r.height - FONT_SZ) / 2),
             FONT_SZ, active ? DARKBLUE : DARKGRAY);
}

static void handleTextInputKey(TextInput& ti) {
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
        if (len > 0) ti.buf[len - 1] = '\0';
    }
}

static void drawTextInput(AppState& state, int idx,
                          Rectangle r, const char* label) {
    TextInput& ti = state.paramInputs[static_cast<size_t>(idx)];
    bool focused  = (state.focusedInput == idx);

    DrawText(label,
             static_cast<int>(r.x),
             static_cast<int>(r.y - 16),
             FONT_SZ, DARKGRAY);

    DrawRectangleRec(r, focused ? Color{255, 255, 220, 255} : WHITE);
    DrawRectangleLinesEx(r, 1.0f, focused ? BLUE : COL_BTN_BORDER);
    DrawText(ti.buf,
             static_cast<int>(r.x + 4),
             static_cast<int>(r.y + (r.height - FONT_SZ) / 2),
             FONT_SZ, BLACK);

    // Blinking cursor
    if (focused) {
        double t = GetTime();
        if ((t - std::floor(t)) < 0.5) {
            int cx = static_cast<int>(r.x + 4) + MeasureText(ti.buf, FONT_SZ);
            DrawLine(cx, static_cast<int>(r.y + 4),
                     cx, static_cast<int>(r.y + r.height - 4), BLACK);
        }
        handleTextInputKey(ti);
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mp = GetMousePosition();
        if (CheckCollisionPointRec(mp, r))
            state.focusedInput = idx;
    }
}

// ── left panel ────────────────────────────────────────────────────────────────
static void drawLeftPanel(AppState& state) {
    Rectangle panel = {0, 0, static_cast<float>(PANEL_W), static_cast<float>(SCREEN_H)};
    DrawRectangleRec(panel, COL_PANEL);
    DrawLine(PANEL_W, 0, PANEL_W, SCREEN_H, COL_BTN_BORDER);

    float x   = 6.0f;
    float y   = 6.0f;
    float fw  = PANEL_W - 12.0f;
    float bh  = 22.0f;
    float bw4 = (fw - 3.0f * 4.0f) / 4.0f;   // 4-column button width

    // ── Signal type buttons ──────────────────────────────────────────────────
    DrawText("Signal Type:", static_cast<int>(x), static_cast<int>(y), FONT_SZ, DARKGRAY);
    y += 18.0f;

    const char* sigLabels[11] = {"S1","S2","S3","S4","S5","S6","S7","S8","S9","S10","S11"};
    for (int i = 0; i < 11; ++i) {
        int col = i % 4, row = i / 4;
        Rectangle br = { x + col*(bw4+4), y + row*(bh+4), bw4, bh };
        SignalType st = static_cast<SignalType>(i + 1);
        drawButton(br, sigLabels[i], state.selectedType == st);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mp = GetMousePosition();
            if (CheckCollisionPointRec(mp, br)) {
                state.selectedType = st;
                state.focusedInput = -1;
            }
        }
    }
    y += 3.0f * (bh + 4.0f) + 8.0f;

    // ── Parameter inputs ─────────────────────────────────────────────────────
    float fh      = 22.0f;
    float fy_step = 40.0f;

    auto field = [&](int idx, const char* label, bool show) {
        if (!show) return;
        drawTextInput(state, idx, {x, y, fw, fh}, label);
        y += fy_step;
    };

    SignalType t = state.selectedType;
    bool needPeriod  = (t==SignalType::S3||t==SignalType::S4||t==SignalType::S5||
                        t==SignalType::S6||t==SignalType::S7||t==SignalType::S8);
    bool needKw      = (t==SignalType::S6||t==SignalType::S7||t==SignalType::S8);
    bool needTs      = (t==SignalType::S9);
    bool needSamples = (t==SignalType::S10);
    bool needFreq    = (t!=SignalType::S10);
    bool needProb    = (t==SignalType::S11);

    field(PI_AMPLITUDE,      "Amplitude (A):",      true);
    field(PI_START_TIME,     "Start Time (t1):",    true);
    field(PI_DURATION,       "Duration (d):",       true);
    field(PI_PERIOD,         "Period (T):",          needPeriod);
    field(PI_DUTY_CYCLE,     "Duty Cycle (kw):",    needKw);
    field(PI_SAMPLING_FREQ,  "Sampling Freq (f):",  needFreq);
    field(PI_STEP_TIME,      "Step Time (ts):",     needTs);
    field(PI_SAMPLE_START,   "Sample Start (n1):",  needSamples);
    field(PI_SAMPLE_STEP,    "Impulse Idx (ns):",   needSamples);
    field(PI_PROBABILITY,    "Probability (p):",    needProb);

    // ── Generate button ──────────────────────────────────────────────────────
    Rectangle genBtn = {x, y, fw, 28.0f};
    drawButton(genBtn, "[ Generate Signal ]", false);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mp = GetMousePosition();
        if (CheckCollisionPointRec(mp, genBtn))
            state.generateSignal();
    }
    y += 34.0f;

    // ── Histogram bins ───────────────────────────────────────────────────────
    DrawText("Histogram bins:", static_cast<int>(x), static_cast<int>(y), FONT_SZ, DARKGRAY);
    y += 18.0f;
    int binOpts[4] = {5,10,15,20};
    float bbw = (fw - 9.0f) / 4.0f;
    for (int i = 0; i < 4; ++i) {
        char lbl[8]; snprintf(lbl, sizeof(lbl), "%d", binOpts[i]);
        Rectangle br = { x + i*(bbw+3), y, bbw, bh };
        drawButton(br, lbl, state.histBins == binOpts[i]);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mp = GetMousePosition();
            if (CheckCollisionPointRec(mp, br)) state.histBins = binOpts[i];
        }
    }
    y += bh + 8.0f;

    // ── Statistics ───────────────────────────────────────────────────────────
    Rectangle statsRect = {x, y, fw, 100.0f};
    DrawRectangleRec(statsRect, {220, 230, 220, 255});
    DrawRectangleLinesEx(statsRect, 1.0f, COL_BTN_BORDER);
    {
        float tx = statsRect.x + 4, ty = statsRect.y + 4;
        const int fs = 13;
        if (!state.statsValid) {
            DrawText("No stats — generate a signal first",
                     static_cast<int>(tx), static_cast<int>(ty), fs, GRAY);
        } else {
            char buf[64];
            snprintf(buf, sizeof(buf), "Mean:     %+.5g", state.statMean);
            DrawText(buf, static_cast<int>(tx), static_cast<int>(ty), fs, BLACK); ty += fs+3;
            snprintf(buf, sizeof(buf), "|Mean|:   %.5g",  state.statAbsMean);
            DrawText(buf, static_cast<int>(tx), static_cast<int>(ty), fs, BLACK); ty += fs+3;
            snprintf(buf, sizeof(buf), "RMS:      %.5g",  state.statRms);
            DrawText(buf, static_cast<int>(tx), static_cast<int>(ty), fs, BLACK); ty += fs+3;
            snprintf(buf, sizeof(buf), "Variance: %.5g",  state.statVar);
            DrawText(buf, static_cast<int>(tx), static_cast<int>(ty), fs, BLACK); ty += fs+3;
            snprintf(buf, sizeof(buf), "Power:    %.5g",  state.statPower);
            DrawText(buf, static_cast<int>(tx), static_cast<int>(ty), fs, BLACK);
        }
    }
    y += 108.0f;

    // ── File operations ──────────────────────────────────────────────────────
    DrawText("File:", static_cast<int>(x), static_cast<int>(y), FONT_SZ, DARKGRAY);
    y += 18.0f;
    float hw = (fw - 4.0f) / 2.0f;
    Rectangle saveBtn = {x,      y, hw, 26.0f};
    Rectangle loadBtn = {x+hw+4, y, hw, 26.0f};
    drawButton(saveBtn, "Save Signal", false);
    drawButton(loadBtn, "Load Signal", false);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mp = GetMousePosition();
        if (CheckCollisionPointRec(mp, saveBtn)) { state.showSaveDialog = true; memset(state.fileNameInput.buf, 0, sizeof(state.fileNameInput.buf)); }
        if (CheckCollisionPointRec(mp, loadBtn)) { state.showLoadDialog = true; memset(state.fileNameInput.buf, 0, sizeof(state.fileNameInput.buf)); }
    }
    y += 32.0f;

    // ── Signal operations ─────────────────────────────────────────────────────
    DrawText("Signal Operations:", static_cast<int>(x), static_cast<int>(y), FONT_SZ, DARKGRAY);
    y += 18.0f;

    // Slot 1
    {
        char lbl[80];
        snprintf(lbl, sizeof(lbl), "Sig1: %s",
                 state.opFile1Path.empty() ? "(none)" : state.opFile1Path.c_str());
        Rectangle r  = {x, y, fw - 54.0f, 22.0f};
        Rectangle lb = {x + fw - 52.0f, y, 52.0f, 22.0f};
        DrawRectangleRec(r, WHITE);
        DrawRectangleLinesEx(r, 1.0f, COL_BTN_BORDER);
        DrawText(lbl, static_cast<int>(r.x+3), static_cast<int>(r.y+4), 11, DARKGRAY);
        drawButton(lb, "Load", false);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mp = GetMousePosition();
            if (CheckCollisionPointRec(mp, lb)) { state.showOp1Dialog = true; memset(state.fileNameInput.buf, 0, sizeof(state.fileNameInput.buf)); }
        }
        y += 26.0f;
    }
    // Slot 2
    {
        char lbl[80];
        snprintf(lbl, sizeof(lbl), "Sig2: %s",
                 state.opFile2Path.empty() ? "(none)" : state.opFile2Path.c_str());
        Rectangle r  = {x, y, fw - 54.0f, 22.0f};
        Rectangle lb = {x + fw - 52.0f, y, 52.0f, 22.0f};
        DrawRectangleRec(r, WHITE);
        DrawRectangleLinesEx(r, 1.0f, COL_BTN_BORDER);
        DrawText(lbl, static_cast<int>(r.x+3), static_cast<int>(r.y+4), 11, DARKGRAY);
        drawButton(lb, "Load", false);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mp = GetMousePosition();
            if (CheckCollisionPointRec(mp, lb)) { state.showOp2Dialog = true; memset(state.fileNameInput.buf, 0, sizeof(state.fileNameInput.buf)); }
        }
        y += 26.0f;
    }

    // Operation selector
    const char* opLabels[4] = {"+", "-", "x", "/"};
    float opbw = (fw - 9.0f) / 4.0f;
    for (int i = 0; i < 4; ++i) {
        Rectangle br = {x + i*(opbw+3), y, opbw, bh};
        drawButton(br, opLabels[i], state.selectedOp == static_cast<Operation>(i));
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mp = GetMousePosition();
            if (CheckCollisionPointRec(mp, br))
                state.selectedOp = static_cast<Operation>(i);
        }
    }
    y += bh + 6.0f;

    Rectangle computeBtn = {x, y, fw, 26.0f};
    drawButton(computeBtn, "[ Compute ]", false);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mp = GetMousePosition();
        if (CheckCollisionPointRec(mp, computeBtn))
            processSignalOperation(state);
    }
    y += 32.0f;

    // Status message
    if (!state.statusMsg.empty()) {
        Color col = (state.statusMsg.find("Error") != std::string::npos ||
                     state.statusMsg.find("error") != std::string::npos) ? RED : DARKGREEN;
        DrawText(state.statusMsg.c_str(),
                 static_cast<int>(x), static_cast<int>(y), 12, col);
    }
}

// ── signal plot ────────────────────────────────────────────────────────────────
static void drawSignalPlot(AppState& state, Rectangle plotRect) {
    DrawRectangleRec(plotRect, COL_PLOT_BG);
    DrawRectangleLinesEx(plotRect, 1.0f, COL_AXIS);

    Signal* sig = state.resultSignal  ? state.resultSignal.get()
                : state.currentSignal ? state.currentSignal.get()
                : nullptr;

    if (!sig || sig->samples.empty()) {
        DrawText("No signal — press [ Generate Signal ]",
                 static_cast<int>(plotRect.x + 20),
                 static_cast<int>(plotRect.y + plotRect.height / 2 - 10),
                 FONT_SZ, GRAY);
        return;
    }

    const float PAD_L = 55.0f, PAD_R = 10.0f, PAD_T = 18.0f, PAD_B = 30.0f;
    float drawW = plotRect.width  - PAD_L - PAD_R;
    float drawH = plotRect.height - PAD_T - PAD_B;
    float ox = plotRect.x + PAD_L;
    float oy = plotRect.y + PAD_T;

    int n = static_cast<int>(sig->samples.size());

    double yMin = *std::min_element(sig->samples.begin(), sig->samples.end());
    double yMax = *std::max_element(sig->samples.begin(), sig->samples.end());
    if (yMax - yMin < 1e-12) { yMin -= 1.0; yMax += 1.0; }
    double yRange = yMax - yMin;

    double tMin   = sig->times.front();
    double tMax   = sig->times.back();
    double tRange = tMax - tMin;
    if (tRange < 1e-12) tRange = 1.0;

    float zoom            = state.plotZoom > 0.01f ? state.plotZoom : 1.0f;
    float visibleDuration = static_cast<float>(tRange) / zoom;
    float scrollT         = state.plotScrollX / drawW * static_cast<float>(tRange) / zoom;
    double tViewMin       = tMin + scrollT;
    double tViewMax       = tViewMin + visibleDuration;
    (void)tViewMax;

    BeginScissorMode(static_cast<int>(ox), static_cast<int>(oy),
                     static_cast<int>(drawW), static_cast<int>(drawH));

    auto toScreenX = [&](double t) -> float {
        return ox + static_cast<float>((t - tViewMin) / visibleDuration) * drawW;
    };
    auto toScreenY = [&](double v) -> float {
        return oy + static_cast<float>(1.0 - (v - yMin) / yRange) * drawH;
    };

    // Zero line
    float zeroY = toScreenY(0.0);
    if (zeroY >= oy && zeroY <= oy + drawH)
        DrawLine(static_cast<int>(ox), static_cast<int>(zeroY),
                 static_cast<int>(ox + drawW), static_cast<int>(zeroY),
                 Color{200, 200, 200, 255});

    if (sig->isDiscrete()) {
        for (int i = 0; i < n; ++i) {
            float sx = toScreenX(sig->times[static_cast<size_t>(i)]);
            float sy = toScreenY(sig->samples[static_cast<size_t>(i)]);
            if (sx < ox - 2 || sx > ox + drawW + 2) continue;
            DrawLine(static_cast<int>(sx), static_cast<int>(zeroY),
                     static_cast<int>(sx), static_cast<int>(sy), COL_SIGNAL);
            DrawCircle(static_cast<int>(sx), static_cast<int>(sy), 3.0f, COL_SIGNAL);
        }
    } else {
        for (int i = 1; i < n; ++i) {
            float x1 = toScreenX(sig->times[static_cast<size_t>(i-1)]);
            float y1 = toScreenY(sig->samples[static_cast<size_t>(i-1)]);
            float x2 = toScreenX(sig->times[static_cast<size_t>(i)]);
            float y2 = toScreenY(sig->samples[static_cast<size_t>(i)]);
            if (x2 < ox - 1 || x1 > ox + drawW + 1) continue;
            DrawLine(static_cast<int>(x1), static_cast<int>(y1),
                     static_cast<int>(x2), static_cast<int>(y2), COL_SIGNAL);
        }
    }

    EndScissorMode();

    // Y-axis ticks and labels
    for (int i = 0; i <= 5; ++i) {
        double v  = yMin + yRange * i / 5.0;
        float  sy = toScreenY(v);
        if (sy < oy || sy > oy + drawH) continue;
        char lbl[20]; snprintf(lbl, sizeof(lbl), "%.3g", v);
        int tw = MeasureText(lbl, 11);
        DrawText(lbl, static_cast<int>(ox - tw - 3), static_cast<int>(sy - 6), 11, DARKGRAY);
        DrawLine(static_cast<int>(ox - 3), static_cast<int>(sy),
                 static_cast<int>(ox),     static_cast<int>(sy), COL_AXIS);
    }

    // X-axis ticks and labels
    for (int i = 0; i <= 6; ++i) {
        double t  = tViewMin + visibleDuration * i / 6.0;
        float  sx = toScreenX(t);
        char lbl[20]; snprintf(lbl, sizeof(lbl), "%.3g", t);
        int tw = MeasureText(lbl, 11);
        DrawText(lbl, static_cast<int>(sx - tw/2.0f),
                 static_cast<int>(oy + drawH + 4), 11, DARKGRAY);
        DrawLine(static_cast<int>(sx), static_cast<int>(oy + drawH),
                 static_cast<int>(sx), static_cast<int>(oy + drawH + 3), COL_AXIS);
    }

    // Axis lines
    DrawLine(static_cast<int>(ox), static_cast<int>(oy),
             static_cast<int>(ox), static_cast<int>(oy + drawH), COL_AXIS);
    DrawLine(static_cast<int>(ox), static_cast<int>(oy + drawH),
             static_cast<int>(ox + drawW), static_cast<int>(oy + drawH), COL_AXIS);

    // Title — store name in a local to avoid dangling pointer from temporary
    std::string titleStr = state.resultSignal  ? "Result Signal"
                         : state.currentSignal ? state.currentSignal->getName()
                         : std::string{};
    DrawText(titleStr.c_str(), static_cast<int>(ox + 4), static_cast<int>(oy + 2), 12, DARKBLUE);

    // Scroll hint
    DrawText("Mouse wheel: scroll  |  Ctrl+wheel: zoom",
             static_cast<int>(ox + drawW - 240),
             static_cast<int>(oy + drawH + 16), 10, GRAY);
}

// ── histogram ──────────────────────────────────────────────────────────────────
static void drawHistogram(AppState& state, Rectangle histRect) {
    DrawRectangleRec(histRect, Color{245, 245, 255, 255});
    DrawRectangleLinesEx(histRect, 1.0f, COL_AXIS);

    Signal* sig = state.resultSignal  ? state.resultSignal.get()
                : state.currentSignal ? state.currentSignal.get()
                : nullptr;

    if (!sig || sig->samples.empty()) {
        DrawText("Histogram: no signal",
                 static_cast<int>(histRect.x + 10),
                 static_cast<int>(histRect.y + 10),
                 FONT_SZ, GRAY);
        return;
    }

    const float PAD_L = 55.0f, PAD_R = 10.0f, PAD_T = 18.0f, PAD_B = 30.0f;
    float drawW = histRect.width  - PAD_L - PAD_R;
    float drawH = histRect.height - PAD_T - PAD_B;
    float ox = histRect.x + PAD_L;
    float oy = histRect.y + PAD_T;

    int bins = state.histBins > 0 ? state.histBins : 10;

    double yMin = *std::min_element(sig->samples.begin(), sig->samples.end());
    double yMax = *std::max_element(sig->samples.begin(), sig->samples.end());
    if (yMax - yMin < 1e-12) { yMin -= 1.0; yMax += 1.0; }
    double binW = (yMax - yMin) / bins;

    std::vector<int> counts(static_cast<size_t>(bins), 0);
    for (double v : sig->samples) {
        int b = static_cast<int>((v - yMin) / binW);
        if (b < 0) b = 0;
        if (b >= bins) b = bins - 1;
        counts[static_cast<size_t>(b)]++;
    }
    int maxCount = *std::max_element(counts.begin(), counts.end());
    if (maxCount == 0) maxCount = 1;

    float bw = drawW / bins;
    for (int i = 0; i < bins; ++i) {
        float barH = static_cast<float>(counts[static_cast<size_t>(i)]) / maxCount * drawH;
        Rectangle bar = { ox + i * bw + 1.0f, oy + drawH - barH, bw - 2.0f, barH };
        DrawRectangleRec(bar, COL_HIST);
        DrawRectangleLinesEx(bar, 1.0f, Color{50, 100, 180, 255});
    }

    // X labels (bin boundaries)
    for (int i = 0; i <= bins; ++i) {
        double v  = yMin + i * binW;
        float  sx = ox + i * bw;
        char lbl[20]; snprintf(lbl, sizeof(lbl), "%.2g", v);
        int tw = MeasureText(lbl, 10);
        DrawText(lbl, static_cast<int>(sx - tw/2.0f),
                 static_cast<int>(oy + drawH + 3), 10, DARKGRAY);
    }

    // Y labels
    for (int i = 0; i <= 4; ++i) {
        int   cnt = maxCount * i / 4;
        float sy  = oy + drawH - static_cast<float>(cnt) / maxCount * drawH;
        char lbl[16]; snprintf(lbl, sizeof(lbl), "%d", cnt);
        int tw = MeasureText(lbl, 10);
        DrawText(lbl, static_cast<int>(ox - tw - 3), static_cast<int>(sy - 5), 10, DARKGRAY);
        DrawLine(static_cast<int>(ox - 3), static_cast<int>(sy),
                 static_cast<int>(ox),     static_cast<int>(sy), COL_AXIS);
    }

    DrawLine(static_cast<int>(ox), static_cast<int>(oy),
             static_cast<int>(ox), static_cast<int>(oy + drawH), COL_AXIS);
    DrawLine(static_cast<int>(ox), static_cast<int>(oy + drawH),
             static_cast<int>(ox + drawW), static_cast<int>(oy + drawH), COL_AXIS);

    DrawText("Histogram", static_cast<int>(ox + 4), static_cast<int>(oy + 2), 12, DARKBLUE);
}

// ── file dialog ────────────────────────────────────────────────────────────────
static void drawFileDialog(AppState& state, const char* title,
                           bool& showFlag,
                           std::function<void(const std::string&)> onOk) {
    if (!showFlag) return;

    Rectangle dlg = {280, 240, 520, 140};
    DrawRectangleRec(dlg, Color{240, 240, 240, 255});
    DrawRectangleLinesEx(dlg, 2.0f, DARKGRAY);
    DrawText(title, static_cast<int>(dlg.x + 10), static_cast<int>(dlg.y + 10), FONT_SZ, BLACK);

    TextInput& ti = state.fileNameInput;
    Rectangle inp = {dlg.x + 10, dlg.y + 40, dlg.width - 20, 28};
    DrawRectangleRec(inp, WHITE);
    DrawRectangleLinesEx(inp, 1.5f, BLUE);
    DrawText(ti.buf,
             static_cast<int>(inp.x + 4),
             static_cast<int>(inp.y + 6),
             FONT_SZ, BLACK);

    handleTextInputKey(ti);

    Rectangle okBtn  = {dlg.x + 10,               dlg.y + 95, 90, 30};
    Rectangle canBtn = {dlg.x + dlg.width - 100,  dlg.y + 95, 90, 30};
    drawButton(okBtn,  "OK",     false);
    drawButton(canBtn, "Cancel", false);

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

// ── main draw entry point ──────────────────────────────────────────────────────
void drawWindow(AppState& state) {
    ClearBackground(LIGHTGRAY);

    drawLeftPanel(state);

    Rectangle plotRect = {
        static_cast<float>(RIGHT_X), 0.0f,
        static_cast<float>(RIGHT_W), static_cast<float>(PLOT_H)
    };
    Rectangle histRect = {
        static_cast<float>(RIGHT_X), static_cast<float>(PLOT_H),
        static_cast<float>(RIGHT_W), static_cast<float>(HIST_H)
    };

    drawSignalPlot(state, plotRect);
    drawHistogram(state,  histRect);

    // ── File dialogs ──────────────────────────────────────────────────────────
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
