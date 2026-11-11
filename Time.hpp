#pragma once
#include <chrono>
using Clock = std::chrono::steady_clock;

enum class Phase { Waiting , Running, Done};
