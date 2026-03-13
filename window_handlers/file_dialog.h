#pragma once
#include <raylib.h>
#include <functional>
#include <string>
#include "../app_state.h"

void drawFileDialog(AppState& state, const char* title,
                    bool& showFlag,
                    std::function<void(const std::string&)> onOk);
