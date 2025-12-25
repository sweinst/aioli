#pragma once

#include <coroutine>
#include <chrono>
using namespace std::chrono_literals;

namespace aio {
    using c_handle = std::coroutine_handle<>;
    using clock = std::chrono::steady_clock;
    using time_point = clock::time_point;
} // namespace aio
