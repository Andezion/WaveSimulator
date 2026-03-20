#pragma once
#include <raylib.h>
#include <functional>
#include <string>
#include "../app_state.h"

// Рисуем диалоговое окно для загрузки или сохранения файла
// Принимает заголовок окна, флаг отображения и функцию-обработчик для кнопки OK, которая получает путь к файлу, выбранному пользователем
void drawFileDialog(AppState& state, const char* title,
                    bool& showFlag,
                    std::function<void(const std::string&)> onOk);
