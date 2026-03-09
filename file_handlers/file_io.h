#pragma once
#include "../waves_math/signal.h"
#include <string>
#include <memory>


bool saveSignal(const Signal& sig, const std::string& path);
std::unique_ptr<Signal> loadSignal(const std::string& path);
