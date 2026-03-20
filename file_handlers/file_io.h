#pragma once
#include "../waves_math/signal.h"
#include <string>
#include <memory>

// Функции для сохранения сигнала в файл и загрузки сигнала из файла
// Эти функции используются в диалогах сохранения и загрузки сигнала в главном окне приложения
bool saveSignal(const Signal& sig, const std::string& path);
std::unique_ptr<Signal> loadSignal(const std::string& path);
