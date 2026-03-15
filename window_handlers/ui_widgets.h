#pragma once
#include <raylib.h>
#include "../app_state.h"

// Функция для отрисовки кнопок (принимает прямоугольник, текст и флаг активности для изменения цвета)
void drawButton(Rectangle r, const char* label, bool active);
// Функция для обработки ввода в текстовое поле (принимает структуру TextInput и обновляет её в зависимости от нажатых клавиш)
void handleTextInputKey(TextInput& ti);
// Функция для отрисовки текстового поля с меткой (принимает состояние приложения, индекс параметра, прямоугольник для отрисовки и текст метки)
void drawTextInput(AppState& state, int idx, Rectangle r, const char* label);
