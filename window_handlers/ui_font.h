#pragma once
#include <raylib.h>

// Наши цвета и размеры для UI элементов
static const Color COL_PANEL = {230, 230, 230, 255};
static const Color COL_PLOT_BG = WHITE;
static const Color COL_SIGNAL = RED;
static const Color COL_AXIS = DARKGRAY;
static const Color COL_BTN_NORMAL = {245, 245, 245, 255};
static const Color COL_BTN_ACTIVE = {180, 210, 255, 255};
static const Color COL_BTN_BORDER = {160, 160, 160, 255};
static const Color COL_HIST = { 80, 160, 220, 255};

static const int PANEL_W = 320; // Ширина панели управления
static const int FONT_SZ = 14; // Размер шрифта для текста в UI

// Инициализация шрифта для рисования текста
void drawText(const char* text, int x, int y, int size, Color color);
// Очистка ресурсов, связанных с шрифтом
int measureText(const char* text, int size);
